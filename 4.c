#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define ROOM_COUNT 974
#define MAX_ROOM_LENGTH 100
#define MAX_CHECKSUM_LENGTH 5

typedef struct Room
{
    char *name;
    int id;
    char *checksum;
} Room;

struct Letter
{
    char letter;
    int count;
};

int part1(FILE *in);
int part2(FILE *in);
Room *getRooms(FILE *in, Room *buff);
int isValid(Room room);
void decrypt(Room room);

int main()
{
    FILE *in = fopen("in4", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    Room rooms[ROOM_COUNT] = {{0}};
    getRooms(in, rooms);
    int sum = 0;
    for (int i = 0; i < ROOM_COUNT; i++)
        if (isValid(rooms[i]))
            sum += rooms[i].id;
    
    for (int i = 0; i < ROOM_COUNT; i++)
    {
        free(rooms[i].name);
        free(rooms[i].checksum);
    }
    return sum;
}

int part2(FILE *in)
{
    Room rooms[ROOM_COUNT] = {{0}};
    getRooms(in, rooms);
    int res = -2;
    for (int i = 0; i < ROOM_COUNT; i++)
        if (isValid(rooms[i]))
        {
            decrypt(rooms[i]);
            if (!strcmp("northpole-object-storage", rooms[i].name))
            {
                res = rooms[i].id;
                break;
            }
        }
    
    for (int i = 0; i < ROOM_COUNT; i++)
    {
        free(rooms[i].name);
        free(rooms[i].checksum);
    }
    return res;
}

Room *getRooms(FILE *in, Room *buff)
{
    size_t n = 0;
    char *line = NULL;

    for (int i = 0; i < ROOM_COUNT; i++)
    {
        buff[i].name = malloc(MAX_ROOM_LENGTH * sizeof(*buff[i].name));
        buff[i].checksum = malloc((MAX_CHECKSUM_LENGTH+1) * sizeof(*buff[i].checksum));
        int nameIndex = 0;
        int checksumIndex = 0;
        getline(&line, &n, in);

        char *ptr;
        for (ptr = line; *ptr > '9' || *ptr < '0'; ptr++)
            buff[i].name[nameIndex++] = *ptr;
        buff[i].name[nameIndex-1] = '\0';

        for (; *ptr != '['; ptr++)
            buff[i].id = buff[i].id * 10 + (*ptr-'0');
        ptr++;
        
        for (; *ptr != ']'; ptr++)
            buff[i].checksum[checksumIndex++] = *ptr;
        buff[i].checksum[checksumIndex] = '\0';
    }

    free(line);
    return buff;
}

int compar(const void *a, const void *b)
{
    struct Letter *aLetter = (struct Letter *) a;
    struct Letter *bLetter = (struct Letter *) b;

    return aLetter->count != bLetter->count
        ? bLetter->count - aLetter->count
        : aLetter->letter - bLetter->letter;
}

int isValid(Room room)
{
    // Get most common letters
    struct Letter letters['z'-'a'+1] = {{0}};
    for (char c = 'a'; c <= 'z'; c++)
        letters[c-'a'].letter = c;
    for (char *ptr = room.name; *ptr != '\0'; ptr++)
        if (*ptr != '-')
            letters[*ptr-'a'].count++;

    char expectedString[MAX_CHECKSUM_LENGTH+1];
    qsort(letters, 'z'-'a'+1, sizeof(*letters), &compar);
    for (int i = 0; i < MAX_CHECKSUM_LENGTH; i++)
        expectedString[i] = letters[i].letter;
    expectedString[MAX_CHECKSUM_LENGTH] = '\0';

    return !strcmp(expectedString, room.checksum);
}

void decrypt(Room room)
{
    for (char *ptr = room.name; *ptr != '\0'; ptr++)
        if (*ptr != '-')
            *ptr = ((*ptr-'a' + room.id) % ('z'-'a'+1)) + 'a';
}

