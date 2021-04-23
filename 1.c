#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define INSTRUCTION_COUNT 168

typedef enum Compass
{
    north,
    east,
    south,
    west
} Compass;

typedef enum Direction
{
    left,
    right
} Direction;

typedef struct Instruction
{
    Direction direction;
    int distance;
} Instruction;

int part1(FILE *in);
int part2(FILE *in);
Instruction *getInstructions(FILE *in, Instruction *buff);
int getDistance(Instruction *instructions, int part);
int min(int a, int b);
int max(int a, int b);
int overlaps(int a1, int b1, int a2, int b2);
int intersects(int x1, int y1a, int y1b, int x2a, int x2b, int y2);

int main()
{
    FILE *in = fopen("in1", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    Instruction instructions[INSTRUCTION_COUNT];
    getInstructions(in, instructions);
    return getDistance(instructions, 1);
}

int part2(FILE *in)
{
    Instruction instructions[INSTRUCTION_COUNT];
    getInstructions(in, instructions);
    return getDistance(instructions, 2);
}

Instruction *getInstructions(FILE *in, Instruction *buff)
{
    size_t n = 0;
    char *line = NULL;
    getline(&line, &n, in);
    char *tok;
    int i = 0;
    char *lineCopy = line;
    while ((tok = strtok(lineCopy, ", ")) != NULL)
    {
        if (strchr(tok, '\n') != NULL)
            *strchr(tok, '\n') = '\0';
        lineCopy = NULL;
        buff[i++] = (Instruction)
        {
            .direction = *tok == 'L' ? left : right,
            .distance = atoi(tok+1)
        };
    }
    free(line);
    return buff;
}

int getDistance(Instruction *instructions, int part)
{
    Compass compass = north;
    int x = 0;
    int y = 0;
    int visittedX[INSTRUCTION_COUNT];
    int visittedY[INSTRUCTION_COUNT];
    visittedX[0] = 0;
    visittedY[0] = 0;
    for (int i = 0; i < INSTRUCTION_COUNT; i++)
    {
        if (instructions[i].direction == left)
            compass = compass == 0 ? 3 : compass-1;
        else
            compass = compass == 3 ? 0 : compass+1;

        switch(compass)
        {
            case north:
                y+=instructions[i].distance;
                break;
            case east:
                x+=instructions[i].distance;
                break;
            case south:
                y-=instructions[i].distance;
                break;
            case west:
                x-=instructions[i].distance;
                break;
        }
        if (part == 2)
        {
            int preX = visittedX[i];
            int preY = visittedY[i];
            for (int j = 1; j < i; j++)
            {
                if (visittedX[j] == visittedX[j-1])
                {
                    if (x == preX)
                    {
                        // Both vertical
                        if (x == visittedX[j] &&
                                overlaps(y, preY, visittedY[j], visittedY[j-1]))
                        {
                            printf("Visitted again with both travelling vertical\n");
                            printf("Back then I travelled from (%d,%d) to (%d,%d)\n",
                                    visittedX[j-1], visittedY[j-1], visittedX[j], visittedY[j]);
                            printf("Now I travel from (%d,%d) to (%d,%d)\n", preX, preY, x, y);
                            // Either north or south
                            if (compass == north)
                                y = min(visittedY[j], visittedY[j-1]);
                            else
                                y = max(visittedY[j], visittedY[j-1]);
                            goto end;
                        }
                    }
                    else
                        // Crossing
                        if (intersects(visittedX[j], visittedY[j], visittedY[j-1], x, preX, y))
                        {
                            printf("Visitted again with now travelling horizontal and formerly travelling vertically\n");
                            printf("Back then I travelled from (%d,%d) to (%d,%d)\n",
                                    visittedX[j-1], visittedY[j-1], visittedX[j], visittedY[j]);
                            printf("Now I travel from (%d,%d) to (%d,%d)\n", preX, preY, x, y);
                            x = visittedX[j];
                            goto end;
                        }
                }
                else
                {
                    if (y == preY)
                    {
                        // Both horizontal
                        if (y == visittedY[j] &&
                                overlaps(x, preX, visittedX[j], visittedX[j-1]))
                        {
                            printf("Visitted again with both travelling horizontally\n");
                            printf("Back then I travelled from (%d,%d) to (%d,%d)\n",
                                    visittedX[j-1], visittedY[j-1], visittedX[j], visittedY[j]);
                            printf("Now I travel from (%d,%d) to (%d,%d)\n", preX, preY, x, y);
                            // Either east or west
                            if (compass == east)
                                x = min(visittedX[j], visittedX[j-1]);
                            else
                                x = max(visittedX[j], visittedX[j-1]);
                            goto end;
                        }
                    }
                    else
                        // Crossing
                        if (intersects(x, y, preY, visittedX[j], visittedX[j-1], visittedY[j]))
                        {
                            printf("Visitted again with now travelling vertically and formerly travelling horizontally\n");
                            printf("Back then I travelled from (%d,%d) to (%d,%d)\n",
                                    visittedX[j-1], visittedY[j-1], visittedX[j], visittedY[j]);
                            printf("Now I travel from (%d,%d) to (%d,%d)\n", preX, preY, x, y);
                            y = visittedY[j];
                            goto end;
                        }
                }
            }
            visittedX[i+1] = x;
            visittedY[i+1] = y;
        }
    }
    if (part == 2)
        fprintf(stderr, "Didn't visit any place twice!\n");

end:
    if (part == 2)
        printf("I already visitted (%d,%d)!\n", x, y);
    if (x < 0)
        x = -x;
    if (y < 0)
        y = -y;
    return x+y;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

int max(int a, int b)
{
    return a < b ? b : a;
}

int overlaps(a1, b1, a2, b2)
{
    return (min(a1, b1) >= min(a2, b2) && min(a1, b1) <= max(a2, b2)) ||
        (max(a1, b1) >= min(a2, b2) && max(a1, b1) <= max(a2, b2));
}

int intersects(x1, y1a, y1b, x2a, x2b, y2)
{
    return x1 >= min(x2a, x2b) && x1 <= max(x2a, x2b) &&
        y2 >= min(y1a, y1b) && y2 <= max(y1a, y1b);
}

