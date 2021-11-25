#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define DISC_COUNT 6

typedef struct
{
    int p0;
    int mod;
} Disc;

int part1(FILE *in);
int part2(FILE *in);
Disc *getDiscs(FILE *in, Disc *buff);
int getT0(Disc disc, int start, int inc);
int lcm(int a, int b);
int gcd(int a, int b);

int main()
{
    FILE *in = fopen("in15", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    Disc buff[DISC_COUNT] = {0};
    Disc *discs = getDiscs(in, buff);
    int inc = 1;
    int t0 = 0;
    for (int i = 0; i < DISC_COUNT; i++)
    {
        t0 = getT0(discs[i], t0, inc);
        inc = lcm(inc, discs[i].mod);
    }
    return t0;
}

int part2(FILE *in)
{
    Disc buff[DISC_COUNT+1] = {0};
    Disc *discs = getDiscs(in, buff);
    discs[DISC_COUNT].p0 = DISC_COUNT+1;
    discs[DISC_COUNT].mod = 11;
    int inc = 1;
    int t0 = 0;
    for (int i = 0; i < DISC_COUNT+1; i++)
    {
        t0 = getT0(discs[i], t0, inc);
        inc = lcm(inc, discs[i].mod);
    }
    return t0;
}

Disc *getDiscs(FILE *in, Disc *buff)
{
    Disc *res = buff;
    char *line = NULL;
    size_t n = 0;

    while (getline(&line, &n, in) && *line != '\0' && *line != '\n')
    {
        *strchr(line, '\n') = '\0';
        Disc disc = {0};
        int i = -1;
        sscanf(line, "Disc #%d has %d positions; at time=0, it is at position %d.",
                &i, &disc.mod, &disc.p0);
        disc.p0 += i;
        *(buff++) = disc;
    }

    free(line);
    return res;
}

int getT0(Disc disc, int start, int inc)
{
    int t;
    for (t = start; (t+disc.p0) % disc.mod != 0; t+= inc);
    if (t > 147483647)
        fprintf(stderr, "Warning - t is close to int max (%d)\n", t);
    return t;
}

int lcm(int a, int b)
{
    return a*b / gcd(a, b);
}
int gcd(int a, int b)
{
    while (b != 0)
    {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

