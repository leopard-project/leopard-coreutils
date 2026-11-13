#include "core.h"

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

const char *core_basename(const char *path)
{
    const char *slash;

    if (path == NULL) return "";
    slash = strrchr(path, '/');
    return slash != NULL ? slash + 1 : path;
}

int core_is_dir(const char *path)
{
    struct stat st;

    return path != NULL && stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

int core_mkdir_p(const char *path)
{
    char buffer[512];
    size_t length;

    if (path == NULL || path[0] == '\0') return -1;
    length = strlen(path);
    if (length >= sizeof(buffer)) return -1;
    memcpy(buffer, path, length + 1);

    for (size_t i = 1; i <= length; i++)
    {
        if (buffer[i] != '/' && buffer[i] != '\0') continue;
        char saved = buffer[i];
        buffer[i] = '\0';
        if (buffer[0] != '\0' && !core_is_dir(buffer))
        {
            if (mkdir(buffer, 0755) != 0 && !core_is_dir(buffer)) return -1;
        }
        buffer[i] = saved;
    }
    return 0;
}

int core_write_all(int fd, const void *buffer, size_t length)
{
    const char *bytes = (const char *)buffer;
    size_t done = 0;

    while (done < length)
    {
        ssize_t written = write(fd, bytes + done, length - done);
        if (written <= 0) return -1;
        done += (size_t)written;
    }
    return 0;
}

int core_copy_file(const char *src, const char *dst)
{
    char buffer[8192];
    struct stat st;
    int in = open(src, O_RDONLY);
    int out;

    if (in < 0) return -1;
    out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) { close(in); return -1; }

    for (;;)
    {
        ssize_t got = read(in, buffer, sizeof(buffer));
        if (got < 0) { close(in); close(out); return -1; }
        if (got == 0) break;
        if (core_write_all(out, buffer, (size_t)got) != 0)
        {
            close(in);
            close(out);
            return -1;
        }
    }
    if (stat(src, &st) == 0) chmod(dst, st.st_mode & 07777);
    close(in);
    close(out);
    return 0;
}

int core_copy_tree(const char *src, const char *dst)
{
    struct stat st;

    if (stat(src, &st) != 0) return -1;
    if (!S_ISDIR(st.st_mode)) return core_copy_file(src, dst);

    if (core_mkdir_p(dst) != 0) return -1;
    DIR *dir = opendir(src);
    if (dir == NULL) return -1;
    struct dirent *entry;
    int result = 0;

    while ((entry = readdir(dir)) != NULL)
    {
        char from[512], to[512];
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
        snprintf(from, sizeof(from), "%s/%s", src, entry->d_name);
        snprintf(to, sizeof(to), "%s/%s", dst, entry->d_name);
        if (core_copy_tree(from, to) != 0) { result = -1; break; }
    }
    closedir(dir);
    return result;
}

int core_rm_rf(const char *path)
{
    struct stat st;

    if (lstat(path, &st) != 0) return -1;
    if (S_ISDIR(st.st_mode))
    {
        DIR *dir = opendir(path);
        if (dir == NULL) return -1;
        struct dirent *entry;
        int result = 0;

        while ((entry = readdir(dir)) != NULL)
        {
            char child[512];
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
            snprintf(child, sizeof(child), "%s/%s", path, entry->d_name);
            if (core_rm_rf(child) != 0) { result = -1; break; }
        }
        closedir(dir);
        if (result != 0) return -1;
        return rmdir(path);
    }
    return unlink(path);
}
