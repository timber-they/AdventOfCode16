#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define FAV_SIZE 4
// Arbitrary, might have to be changed
#define MAX 100
#define DESTX 31
#define DESTY 39

int part1(FILE *in);
int part2(FILE *in);
int isWall(int x, int y, int favNum);
int shortestPath(int x, int y, int *buff);
void print(int *buff);
void fillLocationCount(int x, int y, int *buff, int stepsTaken);
int countLocationCount(int *buff, int maxSteps);

// -1 -> unknown
// -2 -> invalid
// -3 -> inProgress
// -4 -> impossible

int main()
{
    FILE *in = fopen("in13", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

#define GET(x, y, buff) (buff)[(x) + (MAX)*(y)]
int part1(FILE *in)
{
    char favBuff[FAV_SIZE+1];
    if (!fgets(favBuff, FAV_SIZE+1, in))
    {
        fprintf(stderr, "Couldn't read input\n");
        exit(1);
    }
    int favNum = atoi(favBuff);
    int buff[MAX*MAX];
    for (int y = 0; y < MAX; y++)
        for (int x = 0; x < MAX; x++)
            GET(x, y, buff) = isWall(x, y, favNum) ? -2 : -1;
    GET(DESTX, DESTY, buff) = 0;
    return shortestPath(1, 1, buff);
}

int part2(FILE *in)
{
    char favBuff[FAV_SIZE+1];
    if (!fgets(favBuff, FAV_SIZE+1, in))
    {
        fprintf(stderr, "Couldn't read input\n");
        exit(1);
    }
    int favNum = atoi(favBuff);
    int buff[MAX*MAX];
    for (int y = 0; y < MAX; y++)
        for (int x = 0; x < MAX; x++)
            GET(x, y, buff) = isWall(x, y, favNum) ? -2 : -1;
    fillLocationCount(1, 1, buff, 0);
    return countLocationCount(buff, 50);
}

int isWall(int x, int y, int favNum)
{
    int num = x*x + 3*x + 2*x*y + y + y*y + favNum;
    int binaryDigitSum = 0;
    for (int i = 0; i < 32; i++)
        (num & (1<<i)) && binaryDigitSum++;
    return binaryDigitSum & 1;
}

#define MIN(a, b) ((a) >= 0 && (a) < (b) || (b) < 0 ? (a) : (b))
int shortestPath(int x, int y, int *buff)
{
    if (x < 0 || y < 0 || x >= MAX || y >= MAX)
        return -2;
    if (GET(x, y, buff) < -1 || GET(x, y, buff) > -1)
        return GET(x, y, buff);
    GET(x, y, buff) = -3;
    int path0 = shortestPath(x-1, y, buff);
    int path1 = shortestPath(x, y-1, buff);
    int path2 = shortestPath(x+1, y, buff);
    int path3 = shortestPath(x, y+1, buff);
    // No diagonal movement possible
    int min01 = MIN(path0, path1);
    int min23 = MIN(path2, path3);
    int min = MIN(min01, min23);
    int inProgressCount = (path0 == -3) + (path1 == -3) + (path2 == -3) + (path3 == -3);
    GET(x, y, buff) = min < 0
        ? (inProgressCount > 1 ? -1 : -4)
        : min + 1;
    return GET(x, y, buff);
}

void fillLocationCount(int x, int y, int *buff, int stepsTaken)
{
    if (x < 0 || y < 0 || x >= MAX || y >= MAX)
        return;
    if (GET(x, y, buff) == -2)
        // This is a wall
        return;
    if (GET(x, y, buff) != -1 && GET(x, y, buff) <= stepsTaken)
        // Already found an at least as efficient way
        return;
    GET(x, y, buff) = stepsTaken;
    fillLocationCount(x+1, y, buff, stepsTaken+1);
    fillLocationCount(x, y+1, buff, stepsTaken+1);
    fillLocationCount(x-1, y, buff, stepsTaken+1);
    fillLocationCount(x, y-1, buff, stepsTaken+1);
}

int countLocationCount(int *buff, int maxSteps)
{
    int res = 0;
    for (int y = 0; y < MAX; y++)
        for (int x = 0; x < MAX; x++)
            if (GET(x, y, buff) >= 0 && GET(x, y, buff) <= maxSteps)
                res++;
    return res;
}

void print(int *buff)
{
    int printBuff[MAX * MAX];
    for (int i = 0; i < MAX * MAX; i++)
        printBuff[i] = buff[i] < 0 ? buff[i] : -5;
    int x = 1, y = 1;
    GET(DESTX, DESTY, printBuff) = 0;
    while (x != DESTX || y != DESTY)
    {
        int min = -1;
        int path = 0;
        if (x > 0)
        {
            min = GET(x-1, y, buff);
            path = 1;
        }
        if (y > 0 && GET(x, y-1, buff) >= 0 && (GET(x, y-1, buff) < min || min < 0))
        {
            min = GET(x, y-1, buff);
            path = 2;
        }
        if (x < MAX-1 && GET(x+1, y, buff) >= 0 && (GET(x+1, y, buff) < min || min < 0))
        {
            min = GET(x+1, y, buff);
            path = 3;
        }
        if (y < MAX-1 && GET(x, y+1, buff) >= 0 && (GET(x, y+1, buff) < min || min < 0))
        {
            min = GET(x, y+1, buff);
            path = 4;
        }
        GET(x, y, printBuff) = min+1;
        switch(path)
        {
            case 1:
                x--;
                break;
            case 2:
                y--;
                break;
            case 3:
                x++;
                break;
            case 4:
                y++;
                break;
            default:
                fprintf(stderr, "What the heck, that ain't a direction bro! %d\n", path);
                exit(1);
                break;
        }
    }

    for (int y = 0; y < MAX; y++)
    {
        for (int x = 0; x < MAX; x++)
        {
            switch(GET(x, y, printBuff))
            {
                case -1:
                    printf("?? ");
                    break;
                case -2:
                    printf("██ ");
                    break;
                case -3:
                    printf("!! ");
                    break;
                case -4:
                    printf("-- ");
                    break;
                case -5:
                    printf(".. ");
                    break;
                default:
                    if (GET(x, y, printBuff) >= 10)
                        printf("%d ", GET(x, y, printBuff));
                    else
                        printf(" %d ", GET(x, y, printBuff));
                    break;
            }
        }
        printf("\n");
    }
}

