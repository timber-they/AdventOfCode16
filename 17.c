#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <openssl/md5.h>

#define HASH_LENGTH (2 * MD5_DIGEST_LENGTH + 1)
// Arbitrary, might have to be increased
#define MAX_STRING_LENGTH 1000
#define DESTINATION 3

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define IS_OPEN(openDoors,direction) (openDoors) & (1<<(direction))

char *md5(char *string, char *result, int count)
{
    unsigned char hash[MD5_DIGEST_LENGTH];
    char buff[3];
    MD5((unsigned char *) string, strlen(string), hash);
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        snprintf(buff, 3, "%02x", hash[i]);
        strcat(result, buff);
    }

    if (count > 1)
    {
        string = malloc(HASH_LENGTH * sizeof(*string));

        for (int j = 0; j < count - 1; j++)
        {
            *string = '\0';
            strncpy(string, result, HASH_LENGTH);
            *result = '\0';
            *hash = '\0';

            MD5((unsigned char *) string, strlen(string), hash);
            for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
            {
                snprintf(buff, 3, "%02x", hash[i]);
                strcat(result, buff);
            }
        }

        free(string);
    }
    return result;
}

int part1(FILE *in);
int part2(FILE *in);
char getOpenDoors(char *path, char *hashBuff);
int getShortestPath(char *path, char *shortestPath, char *hashBuff, int x, int y, int inLength);
int getLongestPath(char *path, char *longestPath, char *hashBuff, int x, int y, int inLength);

int main()
{
    FILE *in = fopen("in17", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    char path[MAX_STRING_LENGTH] = {0};
    char shortestPath[MAX_STRING_LENGTH] = {0};
    char hashBuff[HASH_LENGTH] = {0};
    if (!fgets(path, MAX_STRING_LENGTH, in))
    {
        fprintf(stderr, "Couldn't read input\n");
        exit(1);
    }
    *strchr(path, '\n') = '\0';
    int inLength = strlen(path);
    int shortestLength = getShortestPath(path, shortestPath, hashBuff, 0, 0, inLength);

    printf("%s\n", shortestPath);
    return shortestLength;
}

int part2(FILE *in)
{
    char path[MAX_STRING_LENGTH] = {0};
    char longestPath[MAX_STRING_LENGTH] = {0};
    char hashBuff[HASH_LENGTH] = {0};
    if (!fgets(path, MAX_STRING_LENGTH, in))
    {
        fprintf(stderr, "Couldn't read input\n");
        exit(1);
    }
    *strchr(path, '\n') = '\0';
    int inLength = strlen(path);
    int longestLength = getLongestPath(path, longestPath, hashBuff, 0, 0, inLength);

    printf("%s\n", longestPath);
    return longestLength;
}

char getOpenDoors(char *path, char *hashBuff)
{
    char *hash = md5(path, hashBuff, 1);
    char res = 0;
    for (int i = 0; i < 4; i++)
        res += hash[i] > 'a' ? (1<<i) : 0;
    *hash = '\0';
    return res;
}

int getShortestPath(char *path, char *shortestPath, char *hashBuff, int x, int y, int inLength)
{
    int currentLength = strlen(path) - inLength;
    int shortestLength = strlen(shortestPath);
    if (shortestLength == 0)
        shortestLength = MAX_STRING_LENGTH + 1;
    if (currentLength >= shortestLength)
        return shortestLength;
    if (x == DESTINATION && y == DESTINATION)
    {
        if (currentLength < shortestLength)
        {
            strncpy(shortestPath, path+inLength, MAX_STRING_LENGTH);
            shortestLength = currentLength;
        }
        return shortestLength;
    }
    if (strlen(path) == MAX_STRING_LENGTH - 1)
        // Max length reached
        return shortestLength;

    char openDoors = getOpenDoors(path, hashBuff);
    // Points to the current '\0'
    char *toAppend = path + inLength + currentLength;
    toAppend[1] = '\0';

    if (y < DESTINATION && IS_OPEN(openDoors, DOWN))
    {
        toAppend[0] = 'D';
        shortestLength = getShortestPath(path, shortestPath, hashBuff,
                x, y+1, inLength);
    }
    if (x < DESTINATION && IS_OPEN(openDoors, RIGHT))
    {
        toAppend[0] = 'R';
        shortestLength = getShortestPath(path, shortestPath, hashBuff,
                x+1, y, inLength);
    }
    if (x > 0 && IS_OPEN(openDoors, LEFT))
    {
        toAppend[0] = 'L';
        shortestLength = getShortestPath(path, shortestPath, hashBuff,
                x-1, y, inLength);
    }
    if (y > 0 && IS_OPEN(openDoors, UP))
    {
        toAppend[0] = 'U';
        shortestLength = getShortestPath(path, shortestPath, hashBuff,
                x, y-1, inLength);
    }

    // Reset string
    toAppend[0] = '\0';
    return shortestLength;
}

int getLongestPath(char *path, char *longestPath, char *hashBuff, int x, int y, int inLength)
{
    int currentLength = strlen(path) - inLength;
    int longestLength = strlen(longestPath);
    if (x == DESTINATION && y == DESTINATION)
    {
        if (currentLength > longestLength)
        {
            strncpy(longestPath, path+inLength, MAX_STRING_LENGTH);
            longestLength = currentLength;
        }
        return longestLength;
    }
    if (strlen(path) == MAX_STRING_LENGTH - 1)
    {
        printf("Max length reached\n");
        // Max length reached
        return longestLength;
    }

    char openDoors = getOpenDoors(path, hashBuff);
    // Points to the current '\0'
    char *toAppend = path + inLength + currentLength;
    toAppend[1] = '\0';

    if (y < DESTINATION && IS_OPEN(openDoors, DOWN))
    {
        toAppend[0] = 'D';
        longestLength = getLongestPath(path, longestPath, hashBuff,
                x, y+1, inLength);
    }
    if (x < DESTINATION && IS_OPEN(openDoors, RIGHT))
    {
        toAppend[0] = 'R';
        longestLength = getLongestPath(path, longestPath, hashBuff,
                x+1, y, inLength);
    }
    if (x > 0 && IS_OPEN(openDoors, LEFT))
    {
        toAppend[0] = 'L';
        longestLength = getLongestPath(path, longestPath, hashBuff,
                x-1, y, inLength);
    }
    if (y > 0 && IS_OPEN(openDoors, UP))
    {
        toAppend[0] = 'U';
        longestLength = getLongestPath(path, longestPath, hashBuff,
                x, y-1, inLength);
    }

    // Reset string
    toAppend[0] = '\0';
    return longestLength;
}

