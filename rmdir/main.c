#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
    int rc = 0;
    (void)envp;
    if (argc < 2) { printf("usage: rmdir dir...\n"); return 2; }
    for (int i = 1; i < argc; i++)
        if (rmdir(argv[i]) != 0) { printf("rmdir: failed %s\n", argv[i]); rc = 1; }
    return rc;
}
