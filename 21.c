#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define GET(password,pos) ((password)->text[((password)->start + (pos)) % (password)->modulo])

typedef struct
{
    char *text;
    int start;
    int modulo;
} Password;

int part1(FILE *in);
int part2(FILE *in);
int scramble(FILE *in, Password *password, char **lineBuff, size_t *n);
void print(Password *password);
int unscramble(FILE *in, Password *password, char **lineBuff, size_t *n);

int main()
{
    FILE *in = fopen("in21", "r");
    printf("Part1: %d\n", part1(in));
    fclose(in);

    FILE *reversedIn = fopen("in21r", "r");
    printf("Part2: %d\n", part2(reversedIn));
    fclose(reversedIn);

    return 0;
}

int part1(FILE *in)
{
    char *text = "abcdefgh";
    char *stored = malloc((strlen(text)+1) * sizeof(*stored));
    strncpy(stored, text, strlen(text)+1);
    Password password = {.text = stored, .start = 0, .modulo = strlen(text)};
    char *lineBuff = NULL;
    size_t n = 0;

    while (scramble(in, &password, &lineBuff, &n)) ;
    print(&password);
    printf("\n");

    free(stored);
    free(lineBuff);
    return strlen(text);
}

int part2(FILE *in)
{
    char *text = "fbgdceah";
    char *stored = malloc((strlen(text)+1) * sizeof(*stored));
    strncpy(stored, text, strlen(text)+1);
    Password password = {.text = stored, .start = 0, .modulo = strlen(text)};
    char *lineBuff = NULL;
    size_t n = 0;

    while (unscramble(in, &password, &lineBuff, &n)) ;
    print(&password);
    printf("\n");

    free(stored);
    free(lineBuff);
    return strlen(text);
}

static void swapPos(Password *password, int posX, int posY)
{
    char buff = GET(password, posX);
    GET(password, posX) = GET(password, posY);
    GET(password, posY) = buff;
}
static void swapLetters(Password *password, char letterX, char letterY)
{
    char *buff = strchr(password->text, letterX);
    *strchr(password->text, letterY) = letterX;
    *buff = letterY;
}
static void rotateLeft(Password *password, int count)
{
    password->start += count % password->modulo;
    if (password->start < 0)
        password->start += password->modulo;
    password->start = password->start % password->modulo;
}
static void rotateRight(Password *password, int count)
{
    password->start -= count % password->modulo;
    if (password->start < 0)
        password->start += password->modulo;
    password->start = password->start % password->modulo;
}
static int rotateBased(Password *password, char letterX)
{
    char *letter = strchr(password->text, letterX);
    int absolutePos = letter - password->text;
    int relativePos = absolutePos - password->start;
    if (relativePos < 0)
        relativePos += password->modulo;
    int count = 1 + relativePos + (relativePos >= 4);
    rotateRight(password, count);
    return count;
}
static void rotateBasedReverse(Password *password, char letterX)
{
    char starting = GET(password, 0);
    int count = 0;
    while (1)
    {
        rotateRight(password, 1);
        count = rotateBased(password, letterX);
        if (GET(password, 0) == starting)
            break;
        // Reverse test rotation
        rotateLeft(password, count);
    }
    // Reverse test rotation
    rotateLeft(password, count);
}
static void reversePositions(Password *password, int posX, int posY)
{
    for (int i = 0; i < (posY - posX + 1) / 2; i++)
        swapPos(password, posX+i, posY-i);
}
static void movePosition(Password *password, int posX, int posY)
{
    if (posY < posX)
        for (int i = posX; i > posY; i--)
            swapPos(password, i, i-1);
    else
        for (int i = posX; i < posY; i++)
            swapPos(password, i, i+1);
}

int scramble(FILE *in, Password *password, char **lineBuff, size_t *n)
{
    if (!getline(lineBuff, n, in))
        return 0;
    char *line = *lineBuff;
    if (*line == '\0' || *line == '\n')
        return 0;
    *strchr(line, '\n') = '\0';

    int count;
    int posX, posY;
    char letterX, letterY;
    if (sscanf(line, "swap position %d with position %d", &posX, &posY) == 2)
    {
        swapPos(password, posX, posY);
        return 1;
    }
    else if (sscanf(line, "swap letter %c with letter %c", &letterX, &letterY) == 2)
    {
        swapLetters(password, letterX, letterY);
        return 2;
    }
    else if (sscanf(line, "rotate left %d steps", &count) == 1)
    {
        rotateLeft(password, count);
        return 3;
    }
    else if (sscanf(line, "rotate right %d steps", &count) == 1)
    {
        rotateRight(password, count);
        return 4;
    }
    else if (sscanf(line, "rotate based on position of letter %c", &letterX) == 1)
    {
        rotateBased(password, letterX);
        return 5;
    }
    else if (sscanf(line, "reverse positions %d through %d", &posX, &posY) == 2)
    {
        reversePositions(password, posX, posY);
        return 6;
    }
    else if (sscanf(line, "move position %d to position %d", &posX, &posY) == 2)
    {
        movePosition(password, posX, posY);
        return 7;
    }
    else
    {
        fprintf(stderr, "Couldn't parse line %s\n", line);
        return -1;
    }
}
int unscramble(FILE *in, Password *password, char **lineBuff, size_t *n)
{
    if (!getline(lineBuff, n, in))
        return 0;
    char *line = *lineBuff;
    if (*line == '\0' || *line == '\n')
        return 0;
    *strchr(line, '\n') = '\0';

    int count;
    int posX, posY;
    char letterX, letterY;
    if (sscanf(line, "swap position %d with position %d", &posX, &posY) == 2)
    {
        swapPos(password, posX, posY);
        return 1;
    }
    else if (sscanf(line, "swap letter %c with letter %c", &letterX, &letterY) == 2)
    {
        swapLetters(password, letterX, letterY);
        return 2;
    }
    else if (sscanf(line, "rotate left %d steps", &count) == 1)
    {
        rotateRight(password, count);
        return 3;
    }
    else if (sscanf(line, "rotate right %d steps", &count) == 1)
    {
        rotateLeft(password, count);
        return 4;
    }
    else if (sscanf(line, "rotate based on position of letter %c", &letterX) == 1)
    {
        rotateBasedReverse(password, letterX);
        return 5;
    }
    else if (sscanf(line, "reverse positions %d through %d", &posX, &posY) == 2)
    {
        reversePositions(password, posX, posY);
        return 6;
    }
    else if (sscanf(line, "move position %d to position %d", &posX, &posY) == 2)
    {
        movePosition(password, posY, posX);
        return 7;
    }
    else
    {
        fprintf(stderr, "Couldn't parse line %s\n", line);
        return -1;
    }
}

void print(Password *password)
{
    for (int i = 0; i < password->modulo; i++)
        printf("%c", GET(password, i));
}

