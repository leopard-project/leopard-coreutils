#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
    char buffer[512];
    (void)argc;
    (void)argv;
    (void)envp;
    if (getcwd(buffer, sizeof(buffer)) == NULL) { printf("pwd: cannot determine cwd\n"); return 1; }
    printf("%s\n", buffer);
    return 0;
}
