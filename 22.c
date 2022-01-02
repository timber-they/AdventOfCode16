#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define X 37
#define Y 25
// Everything bigger than 100T will never be moved
#define BIG 100
#define MAX_DEPTH 10000


#define AT(x,y) ((x) + (y)*(X))
#define ID(eX,eY,wX,wY) ((eX) + (eY)*(X) + (wX)*(X)*(Y) + (wY)*(X)*(Y)*(X))

typedef struct
{
    int used;
    int avail;
} Node;

int part1(FILE *in);
int part2(FILE *in);
Node *read(FILE *in, Node *buff);
int countPairs(Node *nodes, int x, int y);
int countAllPairs(Node *nodes);
int countMoves(Node *nodes, int emptyX, int emptyY, int wantedX, int wantedY,
        int *buff, int depth, int blockingX, int blockingY);
void print(Node *nodes, int emptyX, int emptyY, int wantedX, int wantedY);

int main()
{
    FILE *in = fopen("in22", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    Node buff[X*Y];
    Node *nodes = read(in, buff);
    return countAllPairs(nodes);
}

int part2(FILE *in)
{
    Node buff[X*Y];
    Node *nodes = read(in, buff);
    int emptyX = -1, emptyY = -1;
    for (int x = 0; x < X; x++)
        for (int y = 0; y < Y; y++)
            if (nodes[AT(x,y)].used == 0)
            {
                emptyX = x;
                emptyY = y;
                goto end1;
            }
end1:;
    int blockingX = -1, blockingY = -1;
    int buff2[X*Y*X*Y] = {0};
    for (int x = 0; x < X; x++)
        for (int y = 0; y < Y; y++)
            if (nodes[AT(x,y)].used > BIG)
            {
                blockingX = x;
                blockingY = y;
                goto end2;
            }
end2:;
    return countMoves(nodes, emptyX, emptyY, X-1, 0, buff2, 0, blockingX, blockingY);
}

Node *read(FILE *in, Node *buff)
{
    // Discard first two lines
    fscanf(in, "%*[^\n]\n%*[^\n]\n") == 0 || fprintf(stderr, "PANIC\n");
    int r, num;
    for (int x = 0; x < X; x++)
        for (int y = 0; y < Y; y++)
            (r =
             fscanf(in, "/dev/grid/node-x%*d-y%*d%*[ ]%*dT%*[ ]%dT%*[ ]%dT%*[ ]%*d%%\n%n",
                    &buff[AT(x,y)].used, &buff[AT(x,y)].avail, &num)) == 2 ||
                fprintf(stderr, "PANIC %d, %d (%d,%d)\n", r, num, x, y);
    return buff;
}

int countPairs(Node *nodes, int x, int y)
{
    if (nodes[AT(x,y)].used == 0)
        return 0;
    int res = 0;
    for (int x2 = 0; x2 < X; x2++)
        for (int y2 = 0; y2 < Y; y2++)
            if ((x2 != x || y2 != y) && nodes[AT(x,y)].used <= nodes[AT(x2,y2)].avail)
                res++;
    return res;
}

int countAllPairs(Node *nodes)
{
    int res = 0;
    for (int x = 0; x < X; x++)
        for (int y = 0; y < Y; y++)
            res += countPairs(nodes, x, y);
    return res;
}

int countMoves(Node *nodes, int emptyX, int emptyY, int wantedX, int wantedY,
        int *buff, int depth, int blockingX, int blockingY)
{
    if (depth >= MAX_DEPTH)
    {
        printf("Overflow!\n");
        return -1;
    }
    if (wantedX == 0 && wantedY == 0)
        // We made it!
        return 0;
    if (buff[ID(emptyX, emptyY, wantedX, wantedY)])
        return buff[ID(emptyX, emptyY, wantedX, wantedY)];
    if (nodes[AT(emptyX, emptyY)].used > BIG || nodes[AT(wantedX, wantedY)].used > BIG)
        // Impossible!
        return -1;
    if (wantedY > 0)
        // This would probably be dumb
        return -1;
    buff[ID(emptyX, emptyY, wantedX, wantedY)] = -1;
    int res = 1<<30;
    //printf("Counting moves for: (%d,%d,%d,%d,%d,%d)\n",
            //emptyX, emptyY, wantedX, wantedY, blockingX, blockingY);
    //print(nodes, emptyX, emptyY, wantedX, wantedY);
    int valid = 0;

    if (emptyX > 0 && !(wantedX == emptyX-1 && wantedY == emptyY) &&
            ((emptyY == blockingY+1 && emptyX >= blockingX) ||
             (emptyX >= wantedX && emptyY == wantedY+1)))
    {
        int moves = countMoves(nodes, emptyX-1, emptyY, wantedX, wantedY, buff, depth+1,
                blockingX, blockingY);
        valid = valid && moves >= 0;
        if (moves >= 0 && moves+1 < res)
            res = moves+1;
    }
    if (emptyY > 0 && !(wantedY == emptyY-1 && wantedX == emptyX) &&
            (emptyY > wantedY+1 || (emptyY == wantedY+1 && emptyX <= wantedX-1)))
    {
        int moves = countMoves(nodes, emptyX, emptyY-1, wantedX, wantedY, buff, depth+1,
                blockingX, blockingY);
        valid = valid && moves >= 0;
        if (moves >= 0 && moves+1 < res)
            res = moves+1;
    }
    if (emptyX < X-1 && emptyY == 0 && emptyX < wantedX && emptyY == wantedY)
    {
        int moves = countMoves(nodes, emptyX+1, emptyY,
                wantedX == emptyX+1 && wantedY == emptyY ? emptyX : wantedX, wantedY,
                buff, depth+1, blockingX, blockingY);
        valid = valid && moves >= 0;
        if (moves >= 0 && moves+1 < res)
            res = moves+1;
    }
    if (emptyY < wantedY+1 && !(wantedY == emptyY+1 && wantedX == emptyX) &&
            emptyX == wantedX+1)
    {
        int moves = countMoves(nodes, emptyX, emptyY+1, wantedX, wantedY, buff, depth+1,
                blockingX, blockingY);
        valid = valid && moves >= 0;
        if (moves >= 0 && moves+1 < res)
            res = moves+1;
    }
    
    if (valid)
        buff[ID(emptyX, emptyY, wantedX, wantedY)] = res;
    else
        buff[ID(emptyX, emptyY, wantedX, wantedY)] = 0;
    return res;
}

void print(Node *nodes, int emptyX, int emptyY, int wantedX, int wantedY)
{
    for (int y = 0; y < Y; y++)
    {
        for (int x = 0; x < X; x++)
            printf("%c", x == emptyX && y == emptyY ? '_' :
                    x == wantedX && y == wantedY ? '!' :
                    nodes[AT(x,y)].used > BIG ? '#' :
                    '.');
        printf("\n");
    }
}

