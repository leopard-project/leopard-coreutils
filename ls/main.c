#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static char type_char(mode_t mode)
{
    if (S_ISDIR(mode)) return 'd';
    if (S_ISLNK(mode)) return 'l';
    if (S_ISCHR(mode)) return 'c';
    if (S_ISBLK(mode)) return 'b';
    if (S_ISFIFO(mode)) return 'p';
    if (S_ISSOCK(mode)) return 's';
    return '-';
}

static int show(const char *path, int long_format)
{
    struct stat st;

    if (stat(path, &st) != 0) { printf("ls: cannot access %s\n", path); return 1; }
    if (!S_ISDIR(st.st_mode))
    {
        if (long_format)
            printf("%c %8lld %s\n", type_char(st.st_mode), (long long)st.st_size, path);
        else
            printf("%s\n", path);
        return 0;
    }

    DIR *dir = opendir(path);
    if (dir == NULL) { printf("ls: cannot open %s\n", path); return 1; }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        char full[512];
        struct stat child;
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
        snprintf(full, sizeof(full), "%s/%s", path, entry->d_name);
        if (long_format && stat(full, &child) == 0)
            printf("%c %8lld %s\n", type_char(child.st_mode), (long long)child.st_size, entry->d_name);
        else
            printf("%s\n", entry->d_name);
    }
    closedir(dir);
    return 0;
}

int main(int argc, char *argv[], char *envp[])
{
    int long_format = 0;
    int first = 1;
    int rc = 0;
    (void)envp;

    if (argc > 1 && (!strcmp(argv[1], "-l") || !strcmp(argv[1], "--long")))
    {
        long_format = 1;
        first = 2;
    }
    if (first >= argc)
    {
        if (first == 1) return show(".", long_format);
    }
    for (int i = first; i < argc; i++) rc |= show(argv[i], long_format);
    return rc;
}
