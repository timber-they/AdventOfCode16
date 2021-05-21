#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define WIDTH 50
#define HEIGHT 6

typedef enum Type
{
    set,
    rotateRow,
    rotateColumn,
    nop
} Type;

typedef struct Instruction
{
    Type type;
    int a, b;
} Instruction;

int part1(FILE *in);
int part2(FILE *in);
void execute(int *field, Instruction inst);
Instruction parse(char *line);
void print(int *field);

int main()
{
    FILE *in = fopen("in8", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int field[WIDTH*HEIGHT] = {0};
    size_t n = 0;
    char *line = NULL;
    while (getline(&line, &n, in) && *line != '\0' && *line != '\n')
    {
        *strchr(line, '\n') = '\0';
        Instruction inst = parse(line);
        if (inst.type == nop)
            break;
        execute(field, inst);
    }
    int res = 0;
    for (int i = 0; i < WIDTH*HEIGHT; i++)
        res += field[i];

    free(line);
    return res;
}

int part2(FILE *in)
{
    int field[WIDTH*HEIGHT] = {0};
    size_t n = 0;
    char *line = NULL;
    while (getline(&line, &n, in) && *line != '\0' && *line != '\n')
    {
        *strchr(line, '\n') = '\0';
        Instruction inst = parse(line);
        if (inst.type == nop)
            break;
        execute(field, inst);
    }
    int res = 0;
    for (int i = 0; i < WIDTH*HEIGHT; i++)
        res += field[i];

    free(line);
    print(field);
    return -2;
}

#define AT(x,y) ((y)*(WIDTH)+(x))
void execute(int *field, Instruction inst)
{
    int buff[WIDTH];
    switch(inst.type)
    {
        case set:
            for (int x = 0; x < inst.a; x++)
                for (int y = 0; y < inst.b; y++)
                    field[AT(x,y)] = 1;
            break;
        case rotateRow:
            for (int x = 0; x < WIDTH; x++)
                buff[(x+inst.b)%WIDTH] = field[AT(x,inst.a)];
            for (int x = 0; x < WIDTH; x++)
                field[AT(x,inst.a)] = buff[x];
            break;
        case rotateColumn:
            for (int y = 0; y < HEIGHT; y++)
                buff[(y+inst.b)%HEIGHT] = field[AT(inst.a,y)];
            for (int y = 0; y < HEIGHT; y++)
                field[AT(inst.a,y)] = buff[y];
            break;
        case nop:
            break;
    }
}

Instruction parse(char *line)
{
    Instruction res;
    res.type = nop;
    char *types[nop];
    types[set] = "rect %dx%d";
    types[rotateRow] = "rotate row y=%d by %d";
    types[rotateColumn] = "rotate column x=%d by %d";
    for (Type i = set; i <= rotateColumn; i++)
    {
        if (sscanf(line, types[i], &res.a, &res.b) == 2)
        {
            res.type = i;
            break;
        }
    }
    return res;
}

void print(int *field)
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
            printf("%c", field[AT(x,y)] ? '#' : ' ');
        printf("\n");
    }
}

