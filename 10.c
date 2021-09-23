#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define BOT_COUNT 210
#define OUTPUT_COUNT 21
#define SEARCHED_LOW 17
#define SEARCHED_HIGH 61

typedef struct Bot
{
    int name;
    int lowVal, highVal;
    int lowDest, highDest;
    int newLow, newHigh;
} Bot;

int part1(FILE *in);
int part2(FILE *in);

Bot *getEmptyBots(Bot *buff)
{
    Bot *res = buff;
    for (int i = 0; i < BOT_COUNT; i++)
    {
        res[i].name = i;
        res[i].lowVal = 0;
        res[i].highVal = 0;
        res[i].lowDest = BOT_COUNT;
        res[i].highDest = BOT_COUNT;
        res[i].newLow = 0;
        res[i].newHigh = 0;
    }

    return res;
}

void assignValue(Bot *botRef, int val)
{
    if (botRef->highVal)
        fprintf(stderr, "OH NO, I already had to values!\n");
    if (botRef->lowVal == 0)
        botRef->lowVal = val;
    else if (botRef->lowVal <= val)
        botRef->highVal = val;
    else
    {
        botRef->highVal = botRef->lowVal;
        botRef->lowVal = val;
    }
}

void assignNewValue(Bot *bots, int *outputs, int index, int val)
{
    if (index < 0)
        outputs[-index-1] = val;
    else if (bots[index].highVal)
        fprintf(stderr, "OH NO, I already had two values!\n");
    else if(bots[index].newHigh)
        fprintf(stderr, "OH NO, I'm already getting two values!\n");
    else if (!bots[index].lowVal)
    {
        if (!bots[index].newLow)
            bots[index].newLow = val;
        else if (bots[index].newLow <= val)
            bots[index].newHigh = val;
        else
        {
            bots[index].newHigh = bots[index].newLow;
            bots[index].newLow = val;
        }
    }
    else if (bots[index].lowVal <= val)
        bots[index].newHigh = val;
    else
    {
        bots[index].newHigh = bots[index].lowVal;
        bots[index].newLow = val;
    }
}

Bot *getBots(FILE *in, Bot *buff)
{
    Bot *bots = getEmptyBots(buff);
    char *line = NULL;
    size_t n = 0;
    while (getline(&line, &n, in) && *line != '\0' && *line != '\n')
    {
        *strchr(line, '\n') = '\0';
        int bot = -1, outLow = -1, outHigh = -1, val = -1;
        if (sscanf(line, "bot %d gives low to bot %d and high to bot %d",
                    &bot, &outLow, &outHigh) == 3)
        {
            bots[bot].lowDest = outLow;
            bots[bot].highDest = outHigh;
        }
        else if (sscanf(line, "bot %d gives low to output %d and high to bot %d",
                    &bot, &outLow, &outHigh) == 3)
        {
            bots[bot].lowDest = -(outLow+1);
            bots[bot].highDest = outHigh;
        }
        else if (sscanf(line, "bot %d gives low to bot %d and high to output %d",
                    &bot, &outLow, &outHigh) == 3)
        {
            bots[bot].lowDest = outLow;
            bots[bot].highDest = -(outHigh+1);
        }
        else if (sscanf(line, "bot %d gives low to output %d and high to output %d",
                    &bot, &outLow, &outHigh) == 3)
        {
            bots[bot].lowDest = -(outLow+1);
            bots[bot].highDest = -(outHigh+1);
        }
        else if (sscanf(line, "value %d goes to bot %d", &val, &bot) == 2)
            assignValue(bots+bot, val);
        else
            fprintf(stderr, "Can't parse '%s'!\n", line);
    }
    free(line);
    return bots;
}

void printBot(Bot bot)
{
    printf("%d[%d", bot.name, bot.lowVal);
    if (bot.newLow)
        printf("->%d", bot.newLow);
    printf(",%d", bot.highVal);
    if (bot.newHigh)
        printf("->%d", bot.newHigh);
    printf("]->(%d,%d)\n", bot.lowDest, bot.highDest);
}

int iterate(Bot *bots, int *outputs, int searchedLow, int searchedHigh)
{
    int ret = -1;
    for (int i = 0; i < BOT_COUNT; i++)
        if (bots[i].lowVal == searchedLow && bots[i].highVal == searchedHigh)
            return i+1;
        else if (bots[i].lowVal && bots[i].highVal)
        {
            ret = 0;
            // ACTION
            assignNewValue(bots, outputs, bots[i].lowDest, bots[i].lowVal);
            assignNewValue(bots, outputs, bots[i].highDest, bots[i].highVal);
            bots[i].lowVal = 0;
            bots[i].highVal = 0;
        }

    for (int i = 0; i < BOT_COUNT; i++)
    {
        if (bots[i].newLow)
        {
            bots[i].lowVal = bots[i].newLow;
            bots[i].newLow = 0;
        }
        if (bots[i].newHigh)
        {
            bots[i].highVal = bots[i].newHigh;
            bots[i].newHigh = 0;
        }
    }
    return ret;
}

int main()
{
    FILE *in = fopen("in10", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    Bot bots[BOT_COUNT];
    int outputs[OUTPUT_COUNT];
    getBots(in, bots);
    int res = 0;
    for (int i = 0;; i++)
    {
        res = iterate(bots, outputs, SEARCHED_LOW, SEARCHED_HIGH);
        if (res)
            return res-1;
    }
}

int part2(FILE *in)
{
    return -2;
}

