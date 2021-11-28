#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

typedef struct
{
    int hasPresents;
    int index;
} Elf;

int part1(FILE *in);
int part2(FILE *in);
// Returns the number of elfs with presents left
int findFinalElf(int *elfs, int elfCount);
int findFinalElf2(Elf *elfs, int elfCount);

int main()
{
    FILE *in = fopen("in19", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    int elfCount;
    if (!fscanf(in, "%d", &elfCount))
    {
        fprintf(stderr, "Couldn't read elf count\n");
        exit(1);
    }
    int *elfs = malloc(elfCount * sizeof(*elfs));
    for (int i = 0; i < elfCount; i++)
        elfs[i] = 1;
    int finalElf = findFinalElf(elfs, elfCount);
    free(elfs);
    return finalElf;
}

int part2(FILE *in)
{
    int elfCount;
    if (!fscanf(in, "%d", &elfCount))
    {
        fprintf(stderr, "Couldn't read elf count\n");
        exit(1);
    }
    //elfCount = 12;
    Elf *elfs = malloc(elfCount * sizeof(*elfs));
    for (int i = 0; i < elfCount; i++)
    {
        elfs[i].hasPresents = 1;
        elfs[i].index = i+1;
    }
    int finalElf = findFinalElf2(elfs, elfCount);
    free(elfs);
    return finalElf;
}

int findFinalElf(int *elfs, int elfCount)
{
    int i;
    for (i = 0; i < elfCount; i++)
        if (elfs[i])
            break;
    if (i == elfCount)
    {
        fprintf(stderr, "No elfs with presents left!\n");
        exit(2);
    }
    while (elfs[i] != elfCount)
    {
        int currentPresents = elfs[i];
        elfs[i] += elfs[(i + currentPresents) % elfCount];
        elfs[(i + currentPresents) % elfCount] = 0;
        i = (i + elfs[i]) % elfCount;
    }
    // i is 0-based, elfs are 1-based
    return i+1;
}

// To whomever might read this: If you understand how this works, I admire you
int findFinalElf2(Elf *elfs, int elfCount)
{
    int initialElfCount = elfCount;
    int i = 0;
    int removed = 0;
    int toSkip2 = elfCount;
    while (removed != initialElfCount - 1)
    {
        int oppositeIndex = (i + toSkip2 / 2) % elfCount;
        if (!elfs[oppositeIndex].hasPresents)
        {
            fprintf(stderr, "Opposite index %d isn't even playing anymore! (from %d)\n", oppositeIndex ,i);
            exit(3);
        }
        elfs[oppositeIndex].hasPresents = 0;
        toSkip2++;

        for (i++; !elfs[i%elfCount].hasPresents; i++)
            toSkip2-=2;
        if (toSkip2 < 0)
            toSkip2 += elfCount;
        if (i >= elfCount)
        {
            i = i % elfCount;
            // Looped - resetting
            int k = 0;
            for (int j = 0; j < elfCount; j++)
            {
                if (j == i)
                    i = k;
                if (elfs[j].hasPresents)
                    elfs[k++] = elfs[j];
            }
            elfCount = k;
            toSkip2 = elfCount;
        }
        removed++;
    }

    return elfs[i].index;
}

