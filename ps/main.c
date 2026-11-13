#include <stdio.h>
#include <string.h>
#include "xtuiapi.h"

int main(int argc, char *argv[], char *envp[])
{
    XapiTaskInfo tasks[64];
    (void)argc;
    (void)argv;
    (void)envp;
    memset(tasks, 0, sizeof(tasks));
    UINT64 count = xapi_GetTaskList(tasks, 64);
    printf("  PID   PPID  TID  STATUS  NAME\n");
    for (UINT64 i = 0; i < count && i < 64; i++)
        printf("%5llu %6llu %4llu %7u  %s\n",
               (unsigned long long)tasks[i].pid, (unsigned long long)tasks[i].ppid,
               (unsigned long long)tasks[i].tid, tasks[i].process_status,
               tasks[i].process_name);
    return 0;
}
