#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int contains(const char *line, const char *pattern, int ignore_case)
{
    if (!ignore_case) return strstr(line, pattern) != NULL;

    char lowered_line[1024];
    char lowered_pattern[256];
    size_t i;
    for (i = 0; line[i] && i + 1 < sizeof(lowered_line); i++)
    {
        char c = line[i];
        if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
        lowered_line[i] = c;
    }
    lowered_line[i] = '\0';
    for (i = 0; pattern[i] && i + 1 < sizeof(lowered_pattern); i++)
    {
        char c = pattern[i];
        if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
        lowered_pattern[i] = c;
    }
    lowered_pattern[i] = '\0';
    return strstr(lowered_line, lowered_pattern) != NULL;
}

static int grep_one(const char *path, const char *pattern, int ignore_case, int multiple, int *found)
{
    int fd = path != NULL ? open(path, O_RDONLY) : 0;
    char buffer[4096];
    char line[1024];
    size_t used = 0;

    if (fd < 0) { printf("grep: cannot open %s\n", path); return 1; }
    for (;;)
    {
        ssize_t got = read(fd, buffer, sizeof(buffer));
        if (got <= 0) break;
        for (ssize_t k = 0; k < got; k++)
        {
            if (buffer[k] == '\n' || used + 1 >= sizeof(line))
            {
                line[used] = '\0';
                if (contains(line, pattern, ignore_case))
                {
                    if (multiple && path != NULL) printf("%s:", path);
                    printf("%s\n", line);
                    *found = 1;
                }
                used = 0;
            }
            else line[used++] = buffer[k];
        }
    }
    if (used > 0)
    {
        line[used] = '\0';
        if (contains(line, pattern, ignore_case))
        {
            if (multiple && path != NULL) printf("%s:", path);
            printf("%s\n", line);
            *found = 1;
        }
    }
    if (path != NULL) close(fd);
    return 0;
}

int main(int argc, char *argv[], char *envp[])
{
    int ignore_case = 0, first = 1, found = 0;
    (void)envp;

    if (argc > 1 && !strcmp(argv[1], "-i")) { ignore_case = 1; first = 2; }
    if (argc <= first) { printf("usage: grep [-i] pattern [file...]\n"); return 2; }

    const char *pattern = argv[first];
    int files = argc - (first + 1);
    if (files <= 0)
    {
        grep_one(NULL, pattern, ignore_case, 0, &found);
    }
    else
    {
        for (int i = first + 1; i < argc; i++)
            grep_one(argv[i], pattern, ignore_case, files > 1, &found);
    }
    return found ? 0 : 1;
}
