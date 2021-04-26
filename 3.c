#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

int part1(FILE *in);
int part2(FILE *in);
int isValid(int a, int b, int c);

int main()
{
    FILE *in = fopen("in3", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int a, b, c;
    int possible = 0;
    while (fscanf(in, "%3d  %3d  %3d\n", &a, &b, &c) == 3)
        possible += isValid(a, b, c);

    return possible;
}

int part2(FILE *in)
{
    int a1, a2, a3, b1, b2, b3, c1, c2, c3;
    int possible = 0;
    while (fscanf(in, "%5d%5d%5d\n%5d%5d%5d\n%5d%5d%5d\n", &a1, &b1, &c1, &a2, &b2, &c2, &a3, &b3, &c3) == 9)
        possible += isValid(a1, a2, a3) + isValid(b1, b2, b3) + isValid(c1, c2, c3);

    return possible;
}

int isValid(int a, int b, int c)
{
    return a+b > c &&
        a+c > b &&
        b+c > a;
}

