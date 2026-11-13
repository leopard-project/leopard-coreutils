#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "lib/core.h"

int main(int argc, char *argv[], char *envp[])
{
    (void)envp;
    if (argc < 3) { printf("usage: mv src... dst\n"); return 2; }
    const char *dst = argv[argc - 1];
    int dst_is_dir = core_is_dir(dst);
    int rc = 0;

    for (int i = 1; i < argc - 1; i++)
    {
        char target[512];
        if (dst_is_dir) snprintf(target, sizeof(target), "%s/%s", dst, core_basename(argv[i]));
        else snprintf(target, sizeof(target), "%s", dst);
        if (rename(argv[i], target) != 0)
        {
            if (core_copy_tree(argv[i], target) == 0 && core_rm_rf(argv[i]) == 0) continue;
            printf("mv: failed %s -> %s\n", argv[i], target);
            rc = 1;
        }
    }
    return rc;
}
