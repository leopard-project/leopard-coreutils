#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[])
{
    (void)envp;
    if (argc < 2) { printf("usage: dirname path\n"); return 2; }
    char buffer[512];
    size_t length = strlen(argv[1]);
    if (length >= sizeof(buffer)) length = sizeof(buffer) - 1;
    memcpy(buffer, argv[1], length);
    buffer[length] = '\0';
    while (length > 1 && buffer[length - 1] == '/') buffer[--length] = '\0';
    char *slash = strrchr(buffer, '/');
    if (slash == NULL) { printf(".\n"); return 0; }
    if (slash == buffer) { printf("/\n"); return 0; }
    *slash = '\0';
    printf("%s\n", buffer);
    return 0;
}
