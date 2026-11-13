#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[])
{
    (void)envp;
    if (argc < 2) { printf("usage: basename path [suffix]\n"); return 2; }
    char buffer[512];
    size_t length = strlen(argv[1]);
    if (length >= sizeof(buffer)) length = sizeof(buffer) - 1;
    memcpy(buffer, argv[1], length);
    buffer[length] = '\0';
    while (length > 1 && buffer[length - 1] == '/') buffer[--length] = '\0';
    char *slash = strrchr(buffer, '/');
    const char *name = slash != NULL ? slash + 1 : buffer;
    size_t name_len = strlen(name);
    if (argc > 2)
    {
        size_t suffix_len = strlen(argv[2]);
        if (suffix_len < name_len && !strcmp(name + name_len - suffix_len, argv[2]))
            name_len -= suffix_len;
    }
    fwrite(name, 1, name_len, stdout);
    fputc('\n', stdout);
    return 0;
}
