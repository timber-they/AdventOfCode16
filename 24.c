#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define WIDTH 185
#define HEIGHT 39
#define POIS 7

#define GET(map,x,y) ((map)[(x)+(y)*(WIDTH)])

int part1(FILE *in);
int part2(FILE *in);
int *readMap(FILE *in, int *buff);
int shortestPath(int *map, int x, int y, int found);
void print(int *map);

int main()
{
    FILE *in = fopen("in24", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int *buff = malloc(WIDTH * HEIGHT * sizeof(*buff));
    int *map = readMap(in, buff);
    int x, y;
    for (y = 0; y < WIDTH; y++)
    {
        for (x = 0; x < WIDTH - 1; x++)
            if (!GET(map, x, y))
                break;
        if (!GET(map, x, y))
            break;
    }

    int shortest = shortestPath(map, x, y, 0);

    free(buff);
    return shortest;
}

void print(int *map)
{
    printf("Press the ANY key.\n");
    for (int a = 0; a < HEIGHT; a++)
    {
        for (int b = 0; b < WIDTH; b++)
            printf("%2d", GET(map, b, a));
        printf("\n");
    }
    getc(stdin);
}

int part2(FILE *in)
{
    if (in != NULL)
        in = NULL;
    return -2;
}

int *readMap(FILE *in, int *buff)
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            int c = fgetc(in);
            switch (c)
            {
                case '#':
                    GET(buff, x, y) = -2;
                    break;
                case '.':
                    GET(buff, x, y) = -1;
                    break;
                default:
                    if (c < '0' || c > '9')
                        fprintf(stderr, "Unexpected character: '%c'\n", c);

                    GET(buff, x, y) = c - '0';
                    break;
            }
        }
        // Ignore line break
        if (fgetc(in) != '\n')
            fprintf(stderr, "Expected line break!\n");
    }
    return buff;
}

int try(int *map, int x, int y, int shortest, int found)
{
    int old = GET(map, x, y);
    if (old > 0)
        found++;
    GET(map, x, y) = -10 - found;
    int new = shortestPath(map, x, y, found);
    if (new < shortest)
        shortest = new;
    GET(map, x, y) = old;
    return shortest;
}

int isValid(int *map, int x, int y, int found)
{
    if (x < 0 || x >= WIDTH)
        return 0;
    if (y < 0 || y >= HEIGHT)
        return 0;
    int val = GET(map, x, y);
    if (val > -2)
        return 1;
    return val <= -10 && val + 10 > -found;
}
int shortestPath(int *map, int x, int y, int found)
{
    if (found == POIS)
        return 0;
    int shortest = 1<<30;
    if (isValid(map, x-1, y, found))
        shortest = try(map, x-1, y, shortest, found);
    if (isValid(map, x+1, y, found))
        shortest = try(map, x+1, y, shortest, found);
    if (isValid(map, x, y-1, found))
        shortest = try(map, x, y-1, shortest, found);
    if (isValid(map, x, y+1, found))
        shortest = try(map, x, y+1, shortest, found);
    return shortest+1;
}

