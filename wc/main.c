#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
    int show_lines = 1, show_words = 1, show_bytes = 1;
    int first = 1;
    (void)envp;

    for (; first < argc && argv[first][0] == '-'; first++)
    {
        show_lines = show_words = show_bytes = 0;
        for (const char *p = argv[first] + 1; *p; p++)
        {
            if (*p == 'l') show_lines = 1;
            else if (*p == 'w') show_words = 1;
            else if (*p == 'c') show_bytes = 1;
        }
    }
    if (first >= argc) { printf("usage: wc [-lwc] file...\n"); return 2; }

    unsigned long long total_lines = 0, total_words = 0, total_bytes = 0;
    for (int i = first; i < argc; i++)
    {
        int fd = open(argv[i], O_RDONLY);
        char buffer[4096];
        unsigned long long lines = 0, words = 0, bytes = 0;
        int in_word = 0;
        if (fd < 0) { printf("wc: cannot open %s\n", argv[i]); continue; }
        for (;;)
        {
            ssize_t got = read(fd, buffer, sizeof(buffer));
            if (got <= 0) break;
            bytes += (unsigned long long)got;
            for (ssize_t k = 0; k < got; k++)
            {
                if (buffer[k] == '\n') lines++;
                if (buffer[k] == ' ' || buffer[k] == '\t' || buffer[k] == '\n' || buffer[k] == '\r') in_word = 0;
                else if (!in_word) { in_word = 1; words++; }
            }
        }
        close(fd);
        if (show_lines) printf("%8llu", lines);
        if (show_words) printf("%8llu", words);
        if (show_bytes) printf("%8llu", bytes);
        printf(" %s\n", argv[i]);
        total_lines += lines; total_words += words; total_bytes += bytes;
    }
    if (argc - first > 1)
    {
        if (show_lines) printf("%8llu", total_lines);
        if (show_words) printf("%8llu", total_words);
        if (show_bytes) printf("%8llu", total_bytes);
        printf(" total\n");
    }
    return 0;
}
