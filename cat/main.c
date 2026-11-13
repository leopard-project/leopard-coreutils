#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

static int dump(const char *path)
{
    char buffer[8192];
    int fd = path != NULL ? open(path, O_RDONLY) : 0;
    if (fd < 0) { printf("cat: cannot open %s\n", path); return 1; }
    for (;;)
    {
        ssize_t got = read(fd, buffer, sizeof(buffer));
        if (got <= 0) break;
        if (write(1, buffer, (size_t)got) != got) { if (fd) close(fd); return 1; }
    }
    if (path != NULL) close(fd);
    return 0;
}

int main(int argc, char *argv[], char *envp[])
{
    int rc = 0;
    (void)envp;
    if (argc < 2) return dump(NULL);
    for (int i = 1; i < argc; i++) rc |= dump(argv[i]);
    return rc;
}
