#include <string.h>
#include <stdio.h>
#include "xtuiapi.h"

int main(int argc, char *argv[], char *envp[])
{
    TimeType tm;
    (void)argc;
    (void)argv;
    (void)envp;
    memset(&tm, 0, sizeof(tm));
    xapi_GetTimeX(&tm);
    printf("%04d-%02d-%02d %02d:%02d:%02d\n",
           tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return 0;
}
