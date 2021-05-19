#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

int part1(FILE *in);
int part2(FILE *in);
int isAbba(char *ptr);
int isAba(char *ptr);
int containsBab(char *line, char *aba);
int supportsTls(char *line);
int supportsSsl(char *line);

int main()
{
    FILE *in = fopen("in7", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int res = 0;
    size_t n = 0;
    char *line = NULL;
    while (getline(&line, &n, in) > 0 && *line != '\n' && *line != '\0')
        res += supportsTls(line);

    free(line);
    return res;
}

int part2(FILE *in)
{
    int res = 0;
    size_t n = 0;
    char *line = NULL;
    while (getline(&line, &n, in) > 0 && *line != '\n' && *line != '\0')
        res += supportsSsl(line);

    free(line);
    return res;
}

int isAbba(char *ptr)
{
    for (int i = 0; i < 4; i++)
        if (ptr[-i] < 'a' || ptr[-i] > 'z')
            return 0;
    if (ptr[0] == ptr[-1])
        return 0;
    return ptr[0] == ptr[-3] &&
        ptr[-1] == ptr[-2];
}

int isAba(char *ptr)
{
    for (int i = 0; i < 3; i++)
        if (ptr[-i] < 'a' || ptr[-i] > 'z')
            return 0;
    if (ptr[0] == ptr[-1])
        return 0;
    return ptr[0] == ptr[-2];
}


int supportsTls(char *line)
{
    int isHypernet = 0;
    int res = 0;
    for (int i = 0; i < 3; i++)
        isHypernet += (line[i] == '[') - (line[i] == ']');
    for (char *ptr = line + 3; *ptr != '\0'; ptr++)
    {
        isHypernet += (*ptr == '[') - (*ptr == ']');
        int abba = isAbba(ptr);
        if (abba && isHypernet)
            return 0;
        res |= abba;
    }
    return res;
}

int containsBab(char *line, char *aba)
{
    int isHypernet = 0;
    for (int i = 0; i < 2; i++)
        isHypernet += (line[i] == '[') - (line[i] == ']');
    for (char *ptr = line + 2; *ptr != '\0'; ptr++)
    {
        isHypernet += (*ptr == '[') - (*ptr == ']');
        if (isHypernet && ptr[-2] == aba[1] && ptr[-1] == aba[0] && ptr[0] == aba[1])
            return 1;
    }
    return 0;
}

int supportsSsl(char *line)
{
    int isHypernet = 0;
    for (int i = 0; i < 2; i++)
        isHypernet += (line[i] == '[') - (line[i] == ']');
    for (char *ptr = line + 2; *ptr != '\0'; ptr++)
    {
        isHypernet += (*ptr == '[') - (*ptr == ']');
        if (!isHypernet && isAba(ptr) && containsBab(line, ptr-2))
            return 1;
    }
    return 0;
}

