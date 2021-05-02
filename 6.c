#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

int part1(FILE *in);
int part2(FILE *in);
int getMaximumIndex(int *counts, int len);
int getMinimumIndex(int *counts, int len);
int **getDistribution(FILE *in, int *len);

int main()
{
    FILE *in = fopen("in6", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int len[1];
    int **counts = getDistribution(in, len);
    for (int i = 0; i < *len; i++)
    {
        printf("%c", 'a'+getMaximumIndex(counts[i], 'z'-'a'+1));
        free(counts[i]);
    }
    printf("\n");
    free(counts);

    return 0;
}

int part2(FILE *in)
{
    int len[1];
    int **counts = getDistribution(in, len);
    for (int i = 0; i < *len; i++)
    {
        printf("%c", 'a'+getMinimumIndex(counts[i], 'z'-'a'+1));
        free(counts[i]);
    }
    printf("\n");
    free(counts);

    return 0;
}

int **getDistribution(FILE *in, int *len)
{
    size_t n = 0;
    char *line = NULL;
    int **counts = NULL;
    *len = -1;

    while (getline(&line, &n, in) && *line != '\n')
    {
        *strchr(line, '\n') = '\0';
        if (*len == -1)
        {
            *len = strlen(line);
            counts = malloc(*len * sizeof(*counts));
            for (int i = 0; i < *len; i++)
                counts[i] = calloc(('z'-'a'+1), sizeof(*counts[i]));
        }
        for (int i = 0; i < *len; i++)
            counts[i][line[i]-'a']++;
    }
    free(line);

    return counts;
}

int getMaximumIndex(int *counts, int len)
{
    int maximumIndex = 0;
    for (int i = 1; i < len; i++)
        if (counts[i] > counts[maximumIndex])
            maximumIndex = i;
    return maximumIndex;
}

int getMinimumIndex(int *counts, int len)
{
    int minimumIndex = 0;
    for (int i = 1; i < len; i++)
        if (counts[i] < counts[minimumIndex])
            minimumIndex = i;
    return minimumIndex;
}

