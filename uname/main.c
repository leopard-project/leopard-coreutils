#include <stdio.h>
#include <string.h>
#include "xtuiapi.h"

int main(int argc, char *argv[], char *envp[])
{
    char version[256];
    (void)envp;
    memset(version, 0, sizeof(version));
    xapi_GetSystemVersion((WSTR)version);
    if (argc > 1 && (!strcmp(argv[1], "-a") || !strcmp(argv[1], "--all")))
        printf("XJ380 %s LeopardOS\n", version);
    else
        printf("XJ380\n");
    return 0;
}
