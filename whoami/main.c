#include <stdio.h>
#include "xtuiapi.h"

int main(int argc, char *argv[], char *envp[])
{
    UserInfo user;
    (void)argc;
    (void)argv;
    (void)envp;
    memset(&user, 0, sizeof(user));
    xapi_GetCurrentUser(&user);
    printf("%s\n", user.name[0] ? user.name : "unknown");
    return 0;
}
