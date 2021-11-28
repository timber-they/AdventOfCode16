#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define LINE_LENGTH 100
#define ROWS 40
#define ROWS2 400000

int part1(FILE *in);
int part2(FILE *in);
char *readLine(FILE *in, char *buff);
int countSafety(char *line);
char *calcLine(char *lastLine, char *buff);
int isTrap(char left, char center, char right);

int main()
{
    FILE *in = fopen("in18", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    char buff[LINE_LENGTH+1];
    char *line = readLine(in, buff);
    int res = countSafety(line);
    char newBuff[LINE_LENGTH];
    char *newLine = newBuff;

    for (int i = 0; i < ROWS-1; i++)
    {
        char *otherBuff = line;
        line = calcLine(line, newLine);
        res += countSafety(line);
        newLine = otherBuff;
    }
    
    return res;
}

int part2(FILE *in)
{
    char buff[LINE_LENGTH+1];
    char *line = readLine(in, buff);
    int res = countSafety(line);
    char newBuff[LINE_LENGTH];
    char *newLine = newBuff;

    for (int i = 0; i < ROWS2-1; i++)
    {
        char *otherBuff = line;
        line = calcLine(line, newLine);
        res += countSafety(line);
        newLine = otherBuff;
    }
    
    return res;
}

char *readLine(FILE *in, char *buff)
{
    fgets(buff, LINE_LENGTH, in) ||
        fprintf(stderr, "Couldn't read line\n");
    for (char *ptr = buff; *ptr != '\0'; ptr++)
        *ptr = *ptr == '^';
    return buff;
}

int countSafety(char *line)
{
    int n = 0;
    for (int i = 0; i < LINE_LENGTH; i++)
        n += !line[i];
    return n;
}

char *calcLine(char *lastLine, char *buff)
{
    buff[0] = isTrap(0, lastLine[0], lastLine[1]);
    buff[LINE_LENGTH-1] = isTrap(lastLine[LINE_LENGTH-2], lastLine[LINE_LENGTH-1], 0);
    for (int i = 1; i < LINE_LENGTH-1; i++)
        buff[i] = isTrap(lastLine[i-1], lastLine[i], lastLine[i+1]);
    return buff;
}

int isTrap(char left, char center, char right)
{
    return (left && center && !right) ||
        (!left && center && right) ||
        (left && !center && !right) ||
        (!left && !center && right);
}

