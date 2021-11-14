#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <openssl/md5.h>

#define SALT_LENGTH 8
#define MAX_STRING_LENGTH (SALT_LENGTH+10)
#define HASH_LENGTH (2 * MD5_DIGEST_LENGTH + 1)
#define NEEDED_HASHES 30000

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
int getNthKeyIndex(char *salt, int n, int hashCount);

int main()
{
    FILE *in = fopen("in14", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    char salt[SALT_LENGTH+1];
    if (!fgets(salt, SALT_LENGTH+1, in))
    {
        fprintf(stderr, "Couldn't read salt\n");
        exit(1);
    }
    return getNthKeyIndex(salt, 64, 1);
}

int part2(FILE *in)
{
    char salt[SALT_LENGTH+1];
    if (!fgets(salt, SALT_LENGTH+1, in))
    {
        fprintf(stderr, "Couldn't read salt\n");
        exit(1);
    }
    return getNthKeyIndex(salt, 64, 2017);
}

int getNthKeyIndex(char *salt, int n, int hashCount)
{
    int keysFound = 0;
    char seed[MAX_STRING_LENGTH] = {0};
    char *hashMemory[NEEDED_HASHES];
    for (int i = 0; i < NEEDED_HASHES; i++)
    {
        if (i % 1000 == 0)
            printf("i=%d\n", i);
        hashMemory[i] = malloc(HASH_LENGTH * sizeof(*hashMemory[i]));;
        *hashMemory[i] = '\0';
        if (!snprintf(seed, MAX_STRING_LENGTH, "%s%d", salt, i))
        {
            fprintf(stderr, "Couldn't print\n");
            exit(1);
        }
        md5(seed, hashMemory[i], hashCount);
    }
    for (int i = 0;; i++)
    {
        char *hash = hashMemory[i];
        char trippleChar = '\0';
        for (int k = 2; k < HASH_LENGTH-1; k++)
            if (hash[k] == hash[k-1] && hash[k] == hash[k-2])
            {
                trippleChar = hash[k];
                break;
            }
        if (trippleChar == '\0')
            continue;
        int found = 0;
        for (int j = i+1; j < i+1001 && !found; j++)
        {
            if (j >= NEEDED_HASHES)
            {
                fprintf(stderr, "Need more than %d hashes (validating %d)!\n", j, i);
                exit(1);
            }
            hash = hashMemory[j];
            for (int k = 4; k < HASH_LENGTH-1; k++)
                if (trippleChar == hash[k] &&
                        trippleChar == hash[k-1] &&
                        trippleChar == hash[k-2] &&
                        trippleChar == hash[k-3] &&
                        trippleChar == hash[k-4])
                {
                    keysFound++;
                    found = 1;
                    break;
                }
        }

        if (keysFound == n)
        {
            for (int i = 0; i < NEEDED_HASHES; i++)
                free(hashMemory[i]);
            return i;
        }
    }
}

