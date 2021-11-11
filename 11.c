#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define ORIGINALISOTOPECOUNT 5
#define THINGCOUNT (ISOTOPECOUNT * 2)

typedef enum
{
    thulium,
    plutonium,
    strontium,
    promethium,
    ruthenium,
    elerium,
    dilithium
} Isotope;

typedef enum
{
    nothing,
    generator,
    microchip
} Type;

typedef struct
{
    Type type;
    Isotope isotope;
    int floor;
} Thing;

typedef struct
{
    Thing things[2];
    int floor;
} Elevator;

typedef struct
{
    Elevator elevator;
    // Generators 0-ISOTOPECOUNT-1, Microchips ISOTOPECOUNT-2*ISOTOPECOUNT-1
    Thing *things;
} State;

// -1 -> unknown
// -2 -> invalid
// -3 -> inProgress
// -4 -> impossible

int part1(FILE *in);
int part2(FILE *in);
State parseState(FILE *in, Thing *thingsBuff);
int encode(State state);
int isInvalid(int state);
int getFloor(int state, int i);
int setFloor(int state, int i, int floor);
int getMinimumSteps(int state, int *counts);

int main()
{
    FILE *in = fopen("in11", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int ISOTOPECOUNT = ORIGINALISOTOPECOUNT;

int part1(FILE *in)
{
    Thing things[THINGCOUNT];
    State state = parseState(in, things);
    int encoded = encode(state);
    int *counts = malloc(sizeof(*counts) * (1<<(2*(THINGCOUNT+1))));
    for (int i = 0; i < 1<<(2*(THINGCOUNT+1)); i++)
        counts[i] = isInvalid(i) ? -2 : -1;
    // This is the goal - everything is at the highest floor
    counts[(1<<(2*(THINGCOUNT+1))) - 1] = 0;
    // Calculate counts bottom up
    for (int i = (1<<(2*(THINGCOUNT+1))) - 1; i >= 0; i--)
        counts[i] = getMinimumSteps(i, counts);
    // Obtain result
    int res = getMinimumSteps(encoded, counts);
    free(counts);
    return res;
}

int part2(FILE *in)
{
    ISOTOPECOUNT = ORIGINALISOTOPECOUNT+2;
    Thing things[THINGCOUNT];
    State state = parseState(in, things);
    state.things[5] = (Thing) {generator, elerium, 1};
    state.things[6] = (Thing) {generator, dilithium, 1};
    state.things[12] = (Thing) {microchip, elerium, 1};
    state.things[13] = (Thing) {microchip, elerium, 1};
    int encoded = encode(state);
    int *counts = malloc(sizeof(*counts) * (1<<(2*(THINGCOUNT+1))));
    for (int i = 0; i < 1<<(2*(THINGCOUNT+1)); i++)
        counts[i] = isInvalid(i) ? -2 : -1;
    printf("Initialized\n");
    // This is the goal - everything is at the highest floor
    counts[(1<<(2*(THINGCOUNT+1))) - 1] = 0;
    // Calculate
    int res = getMinimumSteps(encoded, counts);
    free(counts);
    return res;
}

Isotope getIsotope(char *isotopeString)
{
    if (!strcmp("thulium", isotopeString))
        return thulium;
    else if (!strcmp("plutonium", isotopeString))
        return plutonium;
    else if (!strcmp("strontium", isotopeString))
        return strontium;
    else if (!strcmp("promethium", isotopeString))
        return promethium;
    else if (!strcmp("ruthenium", isotopeString))
        return ruthenium;
    else
    {
        printf("Can't parse '%s'\n", isotopeString);
        exit(1);
    }
}

State parseState(FILE *in, Thing *thingsBuff)
{
    char *line = NULL;
    size_t n = 0;
    int floor = 0;
    State state = {0};
    state.things = thingsBuff;
    state.elevator.floor = 1;
    while (getline(&line, &n, in) && *line != '\0' && *line != '\n')
    {
        floor++;
        *strchr(line, '\n') = '\0';
        if (strstr(line, "nothing relevant") != NULL)
            continue;
        *strchr(strstr(line, "contains"), 's') = ',';
        // First part is ignored, as it only contains the floor info, which increments per line
        strtok(line, ",");
        char *token;
        while ((token = strtok(NULL, ",")) != NULL)
        {
            Thing thing = {0};
            thing.floor = floor;
            char *isotopePointer = strstr(token, "a ")+2;
            if (strstr(token, "generator") != NULL)
            {
                *strchr(isotopePointer, ' ') = '\0';
                thing.type = generator;
            }
            else
            {
                *strchr(isotopePointer, '-') = '\0';
                thing.type = microchip;
            }
            thing.isotope = getIsotope(isotopePointer);
            state.things[(thing.type == microchip) * ISOTOPECOUNT + thing.isotope] = thing;
        }
    }
    free(line);
    return state;
}

int isInvalid(int state)
{
    // All chips must be either powered, or in no contact with other RTGs
    for (int i = ISOTOPECOUNT; i < THINGCOUNT; i++)
    {
        if (getFloor(state, i) == getFloor(state, i-ISOTOPECOUNT))
            // RTG is on the same floor as its microchip -> microchip is powered
            continue;
        for (int j = 0; i < ISOTOPECOUNT; i++)
            if (getFloor(state, i) == getFloor(state, j))
                // An RTG is on the same floor as the microchip
                return 1;
    }

    // The elevator can't be alone on its floor
    for (int i = 0; i < THINGCOUNT; i++)
        if (getFloor(state, i) == getFloor(state, THINGCOUNT))
            // Something is on the elevators floor
            return 0;

    return 2;
}

int getFloor(int state, int i)
{
    return (state & (3 << (2*i))) / (1 << (2*i));
}

int setFloor(int state, int i, int floor)
{
    int withoutFloor = state & (~(3 << (2*i)));
    return withoutFloor + (floor << (2*i));
}

int encode(State state)
{
    int floorCode = 0;
    for (int i = 0; i < THINGCOUNT; i++)
        floorCode += (state.things[i].floor-1) << (2*i);
    return floorCode + ((state.elevator.floor-1) << (2*THINGCOUNT));
}

int getMinimumSteps(int state, int *counts)
{
    if (counts[state] < -1)
        // Invalid, inProgress (-> looping) or impossible
        return counts[state];
    if (counts[state] > -1)
        // Already calculated
        return counts[state];
    int minimum = -1;
    counts[state] = -3;
    int elevatorFloor = getFloor(state, THINGCOUNT);
    for (int i = 0; i < THINGCOUNT; i++)
    {
        int thingFloor = getFloor(state, i);
        if (thingFloor != elevatorFloor)
            // The elevator can only move stuff on its floor
            continue;
        int downPossible = elevatorFloor > 0 && i >= ISOTOPECOUNT;;
        int belowEmpty = 1;
        if (downPossible)
            for (int k = 0; k < THINGCOUNT; k++)
                if (getFloor(state, k) < elevatorFloor)
                {
                    belowEmpty = 0;
                    break;
                }
        downPossible = downPossible && !belowEmpty;
        // Firstly we'll move only one thing
        if (downPossible)
        {
            // Move thing down
            int newState = setFloor(setFloor(state, i, thingFloor-1), THINGCOUNT, elevatorFloor-1);
            int newMinimum = getMinimumSteps(newState, counts);
            if (newMinimum > -1 && (newMinimum < minimum || minimum < 0))
                minimum = newMinimum;
        }
        if (elevatorFloor < 3)
        {
            // Move thing up
            int newState = setFloor(setFloor(state, i, thingFloor+1), THINGCOUNT, elevatorFloor+1);
            int newMinimum = getMinimumSteps(newState, counts);
            if (newMinimum > -1 && (newMinimum < minimum || minimum < 0))
                minimum = newMinimum;
        }

        for (int j = i+1; j < THINGCOUNT; j++)
        {
            int secondThingFloor = getFloor(state, j);
            if (secondThingFloor != elevatorFloor)
                continue;
            // Generators should never move downwards
            if (downPossible && j >= ISOTOPECOUNT)
            {
                // Move thing down
                int newState = setFloor(
                        setFloor(
                            setFloor(state, i, thingFloor-1),
                            THINGCOUNT, elevatorFloor-1),
                        j, secondThingFloor-1);
                int newMinimum = getMinimumSteps(newState, counts);
                if (newMinimum > -1 && (newMinimum < minimum || minimum < 0))
                    minimum = newMinimum;
            }
            if (elevatorFloor < 3)
            {
                // Move thing up
                int newState = setFloor(
                        setFloor(
                            setFloor(state, i, thingFloor+1),
                            THINGCOUNT, elevatorFloor+1),
                        j, secondThingFloor+1);
                int newMinimum = getMinimumSteps(newState, counts);
                if (newMinimum > -1 && (newMinimum < minimum || minimum < 0))
                    minimum = newMinimum;
            }
        }
    }
    if (minimum < 0)
        return -4;
    counts[state] = minimum+1;
    return counts[state];
}

