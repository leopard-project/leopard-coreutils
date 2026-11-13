#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
    int rc = 0;
    (void)envp;
    if (argc < 2) { printf("usage: touch file...\n"); return 2; }
    for (int i = 1; i < argc; i++)
    {
        int fd = open(argv[i], O_WRONLY | O_CREAT, 0644);
        if (fd < 0) { printf("touch: cannot create %s\n", argv[i]); rc = 1; }
        else close(fd);
    }
    return rc;
}
