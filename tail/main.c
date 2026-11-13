#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int tail_file(const char *path, int lines)
{
    int fd = path != NULL ? open(path, O_RDONLY) : 0;
    char *data = (char *)malloc(1024 * 1024);
    size_t length = 0;
    if (fd < 0 || data == NULL) { printf("tail: cannot open %s\n", path); if (fd >= 0) close(fd); free(data); return 1; }
    for (;;)
    {
        if (length >= 1024 * 1024) break;
        ssize_t got = read(fd, data + length, 1024 * 1024 - length);
        if (got <= 0) break;
        length += (size_t)got;
    }
    if (path != NULL) close(fd);

    size_t start = 0;
    int seen = 0;
    if (length > 0 && data[length - 1] == '\n') length--;
    for (size_t i = length; i > 0; i--)
    {
        if (data[i - 1] == '\n')
        {
            if (++seen >= lines) { start = i; break; }
        }
    }
    if (length > start) fwrite(data + start, 1, length - start, stdout);
    if (length > start && data[length - 1] != '\n') fputc('\n', stdout);
    free(data);
    return 0;
}

int main(int argc, char *argv[], char *envp[])
{
    int lines = 10, first = 1;
    (void)envp;
    if (argc > 2 && !strcmp(argv[1], "-n")) { lines = atoi(argv[2]); first = 3; }
    if (first >= argc) return tail_file(NULL, lines);
    int rc = 0;
    for (int i = first; i < argc; i++) rc |= tail_file(argv[i], lines);
    return rc;
}
