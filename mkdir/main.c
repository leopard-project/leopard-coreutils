#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "lib/core.h"

int main(int argc, char *argv[], char *envp[])
{
    int parents = 0, first = 1, rc = 0;
    (void)envp;
    if (argc > 1 && (!strcmp(argv[1], "-p") || !strcmp(argv[1], "--parents"))) { parents = 1; first = 2; }
    if (first >= argc) { printf("usage: mkdir [-p] dir...\n"); return 2; }
    for (int i = first; i < argc; i++)
    {
        int error = parents ? core_mkdir_p(argv[i]) : mkdir(argv[i], 0755);
        if (error != 0 && !(parents && core_is_dir(argv[i])))
        {
            printf("mkdir: cannot create %s\n", argv[i]);
            rc = 1;
        }
    }
    return rc;
}
