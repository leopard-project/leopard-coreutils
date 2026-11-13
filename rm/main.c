#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "lib/core.h"

int main(int argc, char *argv[], char *envp[])
{
    int recursive = 0, force = 0, first = 1;
    (void)envp;

    for (; first < argc && argv[first][0] == '-'; first++)
    {
        if (!strcmp(argv[first], "-r") || !strcmp(argv[first], "-R")) recursive = 1;
        else if (!strcmp(argv[first], "-f")) force = 1;
        else if (!strcmp(argv[first], "-rf") || !strcmp(argv[first], "-fr")) { recursive = 1; force = 1; }
    }
    if (first >= argc) { printf("usage: rm [-r] [-f] path...\n"); return 2; }

    int rc = 0;
    for (int i = first; i < argc; i++)
    {
        struct stat st;
        if (stat(argv[i], &st) != 0) { if (!force) { printf("rm: cannot access %s\n", argv[i]); rc = 1; } continue; }
        if (S_ISDIR(st.st_mode) && !recursive) { printf("rm: %s is a directory\n", argv[i]); rc = 1; continue; }
        if (core_rm_rf(argv[i]) != 0 && !force) { printf("rm: failed %s\n", argv[i]); rc = 1; }
    }
    return rc;
}
