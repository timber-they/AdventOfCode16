#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define DISK_SIZE_1 272
#define DISK_SIZE_2 35651584

int part1(FILE *in);
int part2(FILE *in);
char *readState(FILE *in, char *buff, int diskSize);
int iterateDragon(char *dragon, int diskSize);
int iterateHashsum(char *dragon, int length);
void printHashsum(char *dragon, int length);

int main()
{
    FILE *in = fopen("in16", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    char buff[DISK_SIZE_1];
    char *dragon = readState(in, buff, DISK_SIZE_1);
    int length = DISK_SIZE_1;
    while (!iterateDragon(dragon, DISK_SIZE_1)) ;
    while (!((length = iterateHashsum(dragon, length)) & 1)) ;
    printHashsum(dragon, length);
    return length;
}

int part2(FILE *in)
{
    char *buff = malloc(DISK_SIZE_2 * sizeof(*buff));
    char *dragon = readState(in, buff, DISK_SIZE_2);
    int length = DISK_SIZE_2;
    while (!iterateDragon(dragon, DISK_SIZE_2)) ;
    while (!((length = iterateHashsum(dragon, length)) & 1)) ;
    printHashsum(dragon, length);
    free(buff);
    return length;
}

char *readState(FILE *in, char *buff, int diskSize)
{
    fgets(buff, diskSize, in) ||
        fprintf(stderr, "Couldn't read state\n");
    for (int i = 0; i < diskSize; i++)
        buff[i] = 
            buff[i] == '0'
            ? 0
            : (buff[i] == '1'
                    ? 1
                    : -1);
    return buff;
}

int iterateDragon(char *dragon, int diskSize)
{
    int i;
    for (i = 0; i < diskSize && dragon[i] >= 0; i++) ;
    if (i == diskSize)
        // Disk is full
        return 1;
    dragon[i++] = 0;
    int j;
    for (j = 0; j < i-1 && j+i < diskSize; j++)
        dragon[j+i] = !dragon[i-j-2];
    return j+i == diskSize;
}

int iterateHashsum(char *dragon, int length)
{
    int j = 0;
    for (int i = 0; i+1 < length; i+=2)
        dragon[j++] = dragon[i] == dragon[i+1];
    return length / 2;
}

void printHashsum(char *dragon, int length)
{
    for (int i = 0; i < length && dragon[i] >= 0; i++)
        printf("%c", dragon[i] ? '1' : '0');
    printf("\n");
}

