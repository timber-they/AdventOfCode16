#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define LINES 27
#define REGISTERS 4

typedef enum
{
    nop,
    cpy,
    inc,
    tgl,
    dec,
    jnz
} Op;

typedef struct
{
    Op op;
    char p1;
    char p2;
    int n1;
    int n2;
} Inst;

int part1(FILE *in);
int part2(FILE *in);
Inst *parse(FILE *in, Inst *buff);
int *run(Inst *code, int *regs);
Op toggle(Op in);

int main()
{
    FILE *in = fopen("in23", "r");

    printf("Part1: %d\n", part1(in));
    rewind(in);
    printf("Part2: %d\n", part2(in));

    fclose(in);
    return 0;
}

int part1(FILE *in)
{
    Inst buff[LINES+1] = {0};
    Inst *code = parse(in, buff);
    int regs[REGISTERS] = {0};
    regs[0] = 7;
    return *run(code, regs);
}

int part2(FILE *in)
{
    Inst buff[LINES+1] = {0};
    Inst *code = parse(in, buff);
    int regs[REGISTERS] = {0};
    regs[0] = 12;
    return *run(code, regs);
}

Inst *parse(FILE *in, Inst *buff)
{
    Inst *res = buff;
    char *line = NULL;
    size_t n = 0;
    while (getline(&line, &n, in) && *line != '\0' && *line != '\n')
    {
        *strchr(line, '\n') = '\0';
        Inst inst = {0};
        if (!((sscanf(line, "cpy %d %c", &inst.n1, &inst.p2) == 2 && (inst.op = cpy)) ||
            (sscanf(line, "cpy %c %c", &inst.p1, &inst.p2) == 2 && (inst.op = cpy)) ||
            (sscanf(line, "inc %c", &inst.p1) == 1 && (inst.op = inc)) ||
            (sscanf(line, "dec %c", &inst.p1) == 1 && (inst.op = dec)) ||
            (sscanf(line, "tgl %c", &inst.p1) == 1 && (inst.op = tgl)) ||
            (sscanf(line, "jnz %d %d", &inst.n1, &inst.n2) == 2 && (inst.op = jnz)) ||
            (sscanf(line, "jnz %d %c", &inst.n1, &inst.p2) == 2 && (inst.op = jnz)) ||
            (sscanf(line, "jnz %c %d", &inst.p1, &inst.n2) == 2 && (inst.op = jnz)) ||
            (sscanf(line, "jnz %c %c", &inst.p1, &inst.p2) == 2 && (inst.op = jnz))))
            fprintf(stderr, "Couldn't parse line '%s'\n", line);
        *(buff++) = inst;
    }

    free(line);
    return res;
}

#define GET(par, inst, regs) ((inst)->p##par ? (regs)[(inst)->p##par-'a'] : (inst)->n##par)
int *run(Inst *code, int *regs)
{
    Inst *inst;
    int itCount = 0;
    for (inst = code; inst != NULL; inst++)
    {
        itCount++;
        switch(inst->op)
        {
            case nop:
                return regs;
            case cpy:
                if (inst->p2 < 'a')
                    break;
                regs[inst->p2-'a'] = GET(1, inst, regs);
                break;
            case inc:
                if (inst->p1 < 'a')
                    break;
                regs[inst->p1-'a']++;
                break;
            case dec:
                if (inst->p1 < 'a')
                    break;
                regs[inst->p1-'a']--;
                break;
            case tgl:
            {
                Inst *ptr = inst + GET(1, inst, regs);
                if (ptr - code >= LINES || ptr - code < 0)
                    break;
                ptr->op = toggle(ptr->op);
                break;
            }
            case jnz:
                // Jump one less, as the for loop increments too
                if (!(GET(1, inst, regs) && (inst += GET(2, inst, regs) - 1)))
                    break;
                break;
            default:
                fprintf(stderr, "Unknown operation: %d\n", inst->op);
                break;
        }
    }
    return regs;
}

Op toggle(Op in)
{
    switch(in)
    {
        case nop:
            return nop;
        case inc:
            return dec;
        case dec:
        case tgl:
            return inc;
        case jnz:
            return cpy;
        case cpy:
            return jnz;
        default:
            fprintf(stderr, "I forgot the operator %d!\n", in);
            return in;
    }
}

