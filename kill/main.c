#include <stdio.h>
#include <stdlib.h>
#include "xtuiapi.h"

int main(int argc, char *argv[], char *envp[])
{
    (void)envp;
    if (argc < 2) { printf("usage: kill pid\n"); return 2; }
    int pid = atoi(argv[1]);
    if (pid <= 0) { printf("kill: invalid pid\n"); return 2; }
    UINT64 rc = xapi_KillProcess((UINT64)pid);
    if ((int64_t)rc < 0) { printf("kill: no such process %d\n", pid); return 1; }
    return 0;
}
