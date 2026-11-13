#include <stdio.h>
#include "xtuiapi.h"

int main(int argc, char *argv[], char *envp[])
{
    (void)argc;
    (void)argv;
    (void)envp;
    UINT64 kb = xapi_GetMemorySize();
    printf("              total\n");
    printf("Mem:      %8llu MiB\n", (unsigned long long)(kb / 1024));
    return 0;
}
