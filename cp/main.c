#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "lib/core.h"

int main(int argc, char *argv[], char *envp[])
{
    int recursive = 0;
    int first = 1;
    (void)envp;

    if (argc > 1 && !strcmp(argv[1], "-r")) { recursive = 1; first = 2; }
    else if (argc > 1 && !strcmp(argv[1], "-R")) { recursive = 1; first = 2; }
    if (argc - first < 2) { printf("usage: cp [-r] src... dst\n"); return 2; }

    const char *dst = argv[argc - 1];
    int dst_is_dir = core_is_dir(dst);
    int rc = 0;
    for (int i = first; i < argc - 1; i++)
    {
        char target[512];
        struct stat st;
        if (stat(argv[i], &st) != 0) { printf("cp: cannot stat %s\n", argv[i]); rc = 1; continue; }
        if (S_ISDIR(st.st_mode) && !recursive) { printf("cp: %s is a directory\n", argv[i]); rc = 1; continue; }
        if (dst_is_dir) snprintf(target, sizeof(target), "%s/%s", dst, core_basename(argv[i]));
        else snprintf(target, sizeof(target), "%s", dst);
        if (core_copy_tree(argv[i], target) != 0) { printf("cp: failed %s -> %s\n", argv[i], target); rc = 1; }
    }
    return rc;
}
