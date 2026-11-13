#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "libsys.h"

struct core_statfs {
    uint64_t f_type, f_bsize, f_blocks, f_bfree, f_bavail, f_files, f_ffree;
    int32_t  f_fsid[2];
    uint64_t f_namelen, f_frsize, f_flags, f_spare[4];
};

int main(int argc, char *argv[], char *envp[])
{
    struct core_statfs st;
    const char *path = argc > 1 ? argv[1] : "/";
    (void)envp;
    memset(&st, 0, sizeof(st));
    if ((int64_t)enter_syscall(SYS_STATFS, (uint64_t)path, (uint64_t)&st, 0, 0, 0, 0) < 0)
    {
        printf("df: cannot stat %s\n", path);
        return 1;
    }
    uint64_t total = st.f_blocks * st.f_bsize / (1024 * 1024);
    uint64_t free  = st.f_bfree  * st.f_bsize / (1024 * 1024);
    printf("Filesystem  1M-blocks  1M-free  Mounted on\n");
    printf("%-12s %9llu %8llu  %s\n", "root", (unsigned long long)total,
           (unsigned long long)free, path);
    return 0;
}
