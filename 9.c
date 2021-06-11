#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

int part1(FILE *in);
size_t part2(FILE *in);
int getDecompressedLength(char *compressed);
size_t getDecompressedLength2(char *compressed);

int main()
{
    FILE *in = fopen("in9", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %ld\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    size_t n = 0;
    char *line = NULL;
    if (!getline(&line, &n, in))
        fprintf(stderr, "Couldn't read file!\n");
    *strchr(line, '\n') = '\0';
    int res = getDecompressedLength(line);
    free(line);
    return res;
}

size_t part2(FILE *in)
{
    size_t n = 0;
    char *line = NULL;
    if (!getline(&line, &n, in))
        fprintf(stderr, "Couldn't read file!\n");
    *strchr(line, '\n') = '\0';
    size_t res = getDecompressedLength2(line);
    free(line);
    return res;
}

int getDecompressedLength(char *compressed)
{
    int count = 0;
    for (char *ptr = compressed; *ptr != '\0'; ptr++)
    {
        if (*ptr == '(')
        {
            ptr++;
            char *end = strchr(ptr, ')');
            *end = '\0';
            int subsequent = 0, repeat = 0;
            if (sscanf(ptr, "%dx%d", &subsequent, &repeat) != 2)
                fprintf(stderr, "Unexpected format: %s\n", ptr);
            count += subsequent * repeat;
            ptr = end + subsequent;
        }
        else
            count++;
    }
    return count;
}

size_t getDecompressedLength2(char *compressed)
{
    size_t n = strlen(compressed);
    size_t count = 0;
    size_t *multipliers = malloc(n * sizeof(*multipliers));
    for (int i = 0; i < n; i++)
        multipliers[i] = 1;
    int i = 0;
    for (char *ptr = compressed; *ptr != '\0'; ptr++, i++)
    {
        if (*ptr == '(')
        {
            ptr++;
            char *end = strchr(ptr, ')');
            *end = '\0';
            int subsequent = 0, repeat = 0;
            if (sscanf(ptr, "%dx%d", &subsequent, &repeat) != 2)
                fprintf(stderr, "Unexpected format: %s\n", ptr);
            i += end - ptr + 1;
            for (int j = 0; j < subsequent; j++)
                multipliers[i+j+1] *= repeat;
            ptr = end;
        }
        else
            count += multipliers[i];
    }
    free(multipliers);
    return count;
}

