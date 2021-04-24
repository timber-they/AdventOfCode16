#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define VIRTUAL_KEYBOARD_SIZE 7

char keypad[VIRTUAL_KEYBOARD_SIZE*VIRTUAL_KEYBOARD_SIZE] =
{
    '0', '0', '0', '0', '0', '0', '0',
    '0', '0', '0', '1', '0', '0', '0',
    '0', '0', '2', '3', '4', '0', '0',
    '0', '5', '6', '7', '8', '9', '0',
    '0', '0', 'A', 'B', 'C', '0', '0',
    '0', '0', '0', 'D', '0', '0', '0',
    '0', '0', '0', '0', '0', '0', '0'
};

int part1(FILE *in);
char *part2(FILE *in, char *buff);
int followInstructions(int initial, char *instructions);
int followInstruction(int current, char instruction);
char *followInstructions2(char *initial, char *instructions);
char *followInstruction2(char *current, char instruction);

int main()
{
    FILE *in = fopen("in2", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    char buff[6] = {0};
    printf("Part2: %s\n", part2(in, buff));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    size_t n = 0;
    char *line = NULL;
    int res = 0;
    int current = 5;
    while (getline(&line, &n, in) > 0 && *line != '\0' && *line != '\n')
        res = res * 10 + (current = followInstructions(current, line));

    free(line);
    return res;
}

char *part2(FILE *in, char *buff)
{
    size_t n = 0;
    char *line = NULL;
    char *current = strchr(keypad, '5');
    int i = 0;
    while (getline(&line, &n, in) > 0 && *line != '\0' && *line != '\n')
        buff[i++] = *(current = followInstructions2(current, line));

    free(line);
    return buff;
}

int followInstructions(int initial, char *instructions)
{
    int res = initial;
    for (char *ptr = instructions; *ptr != '\0' && *ptr != '\n'; ptr++)
        res = followInstruction(res, *ptr);
    return res;
}

int followInstruction(int current, char instruction)
{
    int res = current;
    switch(instruction)
    {
        case 'U':
            if (res > 3)
                res -= 3;
            break;
        case 'D':
            if (res < 7)
                res += 3;
            break;
        case 'L':
            if (res % 3 != 1)
                res--;
            break;
        case 'R':
            if (res % 3 != 0)
                res++;
            break;
    }
    return res;
}

char *followInstructions2(char *initial, char *instructions)
{
    char *res = initial;
    for (char *ptr = instructions; *ptr != '\0' && *ptr != '\n'; ptr++)
        res = followInstruction2(res, *ptr);
    return res;
}

char *followInstruction2(char *current, char instruction)
{
    char *res = current;
    switch(instruction)
    {
        case 'U':
            res -= VIRTUAL_KEYBOARD_SIZE;
            break;
        case 'D':
            res += VIRTUAL_KEYBOARD_SIZE;
            break;
        case 'L':
            res--;
            break;
        case 'R':
            res++;
            break;
    }
    return *res == '0' ? current : res;
}

