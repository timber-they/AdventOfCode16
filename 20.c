#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define BLACKLIST_LENGTH 1075

typedef struct
{
    long lower;
    long higher;
} Range;

static int cmpRange(const void *p1, const void *p2)
{
    long diff = ((Range *) p1)->lower - ((Range *) p2)->lower;
    if (diff < 0)
        return -1;
    return diff > 0;
}

int part1(FILE *in);
int part2(FILE *in);
void readBlacklist(FILE *in, Range *ranges);
int withoutContaining(Range *ranges, Range *buff);
int isContained(Range theRange, Range *ranges);
int isInAnyRange(long value, Range *ranges, int count);
long findNextRangeStart(long value, Range *ranges, int count);

int main()
{
    FILE *in = fopen("in20", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    Range ranges[BLACKLIST_LENGTH];
    readBlacklist(in, ranges);
    qsort(ranges, BLACKLIST_LENGTH, sizeof(Range), cmpRange);
    Range cleaned[BLACKLIST_LENGTH];
    int cleanedCount = withoutContaining(ranges, cleaned);
    for (int i = 0; i < cleanedCount; i++)
        if (!isInAnyRange(cleaned[i].higher+1, cleaned, cleanedCount))
            return cleaned[i].higher+1;
    return -1;
}

int part2(FILE *in)
{
    Range ranges[BLACKLIST_LENGTH];
    readBlacklist(in, ranges);
    qsort(ranges, BLACKLIST_LENGTH, sizeof(Range), cmpRange);
    Range cleaned[BLACKLIST_LENGTH];
    int cleanedCount = withoutContaining(ranges, cleaned);
    int allowedCount = 0;
    for (int i = 0; i < cleanedCount; i++)
        if (!isInAnyRange(cleaned[i].higher+1, cleaned, cleanedCount))
            allowedCount += findNextRangeStart(cleaned[i].higher+1, cleaned, cleanedCount)
                - (cleaned[i].higher+1);
    return allowedCount;
}

void readBlacklist(FILE *in, Range *ranges)
{
    for (int i = 0; i < BLACKLIST_LENGTH; i++)
        if (!fscanf(in, "%ld-%ld", &ranges[i].lower, &ranges[i].higher))
            fprintf(stderr, "Couldn't parse index %d!\n", i);
}

int withoutContaining(Range *ranges, Range *buff)
{
    int j = 0;
    buff[j++] = ranges[0];
    for (int i = 1; i < BLACKLIST_LENGTH; i++)
    {
        if (!isContained(ranges[i], ranges))
            buff[j++] = ranges[i];
    }
    return j;
}

int isContained(Range theRange, Range *ranges)
{
    for (int i = 0; i < BLACKLIST_LENGTH && ranges[i].lower < theRange.lower; i++)
        if (ranges[i].higher >= theRange.higher)
            return 1;
    return 0;
}

int isInAnyRange(long value, Range *ranges, int count)
{
    for (int i = 0; i < count && ranges[i].lower <= value; i++)
        if (ranges[i].higher >= value)
            return 1;
    return 0;
}

long findNextRangeStart(long value, Range *ranges, int count)
{
    int i;
    for (i = 0; i < count && ranges[i].lower <= value; i++);
    if (i == count)
        return 4294967295+1;
    return ranges[i].lower;
}

