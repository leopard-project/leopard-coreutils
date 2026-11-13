#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int head_file(const char *path, int lines)
{
    int fd = path != NULL ? open(path, O_RDONLY) : 0;
    char buffer[4096];
    int printed = 0;
    if (fd < 0) { printf("head: cannot open %s\n", path); return 1; }
    if (lines <= 0) { if (path) close(fd); return 0; }
    while (printed < lines)
    {
        ssize_t got = read(fd, buffer, sizeof(buffer));
        if (got <= 0) break;
        for (ssize_t i = 0; i < got; i++)
        {
            fputc(buffer[i], stdout);
            if (buffer[i] == '\n' && ++printed >= lines) break;
        }
    }
    if (path != NULL) close(fd);
    return 0;
}

int main(int argc, char *argv[], char *envp[])
{
    int lines = 10, first = 1;
    (void)envp;
    if (argc > 2 && !strcmp(argv[1], "-n")) { lines = atoi(argv[2]); first = 3; }
    if (first >= argc) return head_file(NULL, lines);
    int rc = 0;
    for (int i = first; i < argc; i++) rc |= head_file(argv[i], lines);
    return rc;
}
