#include <stdio.h>
#include <stdlib.h>
#include "xtuiapi.h"

int main(int argc, char *argv[], char *envp[])
{
    (void)envp;
    if (argc < 2) { printf("usage: sleep seconds\n"); return 2; }
    long seconds = atoi(argv[1]);
    if (seconds < 0) seconds = 0;
    xapi_Sleep((UINT64)seconds * 1000ULL);
    return 0;
}
