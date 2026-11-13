#include <stdio.h>
#include <string.h>

static void unescape(const char *text)
{
    for (const char *p = text; *p; p++)
    {
        if (*p != '\\' || p[1] == '\0') { fputc(*p, stdout); continue; }
        p++;
        switch (*p)
        {
        case 'n': fputc('\n', stdout); break;
        case 't': fputc('\t', stdout); break;
        case 'r': fputc('\r', stdout); break;
        case '\\': fputc('\\', stdout); break;
        default: fputc('\\', stdout); fputc(*p, stdout); break;
        }
    }
}

int main(int argc, char *argv[], char *envp[])
{
    int newline = 1;
    int first = 1;
    (void)envp;

    if (argc > 1 && !strcmp(argv[1], "-n")) { newline = 0; first = 2; }
    for (int i = first; i < argc; i++)
    {
        if (i > first) fputc(' ', stdout);
        unescape(argv[i]);
    }
    if (newline) fputc('\n', stdout);
    return 0;
}
