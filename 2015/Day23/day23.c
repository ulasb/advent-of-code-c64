#include <stdio.h>
#include <string.h>
#include <conio.h>

/* Instruction Opcodes */
#define OP_HLF 0
#define OP_TPL 1
#define OP_INC 2
#define OP_JMP 3
#define OP_JIE 4
#define OP_JIO 5

/* Registers */
#define REG_A 0
#define REG_B 1

typedef struct {
    unsigned char op;
    unsigned char reg;
    int offset;
} Instruction;

/* 
 * Synthetic test program that exercises all instructions.
 * This ensures that hlf, tpl, inc, jmp, jie, and jio all work as expected.
 */
Instruction program[] = {
    {OP_INC, REG_A, 0},   /* 0: inc a */
    {OP_JIO, REG_A, 3},   /* 1: jio a, +3 */
    {OP_TPL, REG_A, 0},   /* 2: tpl a */
    {OP_INC, REG_A, 0},   /* 3: inc a */
    {OP_INC, REG_A, 0},   /* 4: inc a */
    {OP_JIE, REG_A, 3},   /* 5: jie a, +3 */
    {OP_INC, REG_B, 0},   /* 6: inc b */
    {OP_INC, REG_B, 0},   /* 7: inc b */
    {OP_TPL, REG_A, 0},   /* 8: tpl a */
    {OP_HLF, REG_A, 0},   /* 9: hlf a */
    {OP_INC, REG_B, 0},   /* 10: inc b */
    {OP_JMP, 0, 2},       /* 11: jmp +2 (FIXED: offset was in wrong field) */
    {OP_INC, REG_B, 0},   /* 12: inc b */
    {OP_INC, REG_A, 0},   /* 13: inc a */
    {OP_JIE, REG_A, -4},  /* 14: jie a, -4 */
    {OP_INC, REG_B, 0}    /* 15: inc b */
};

#define PROG_LEN (sizeof(program) / sizeof(Instruction))

/* Global registers to avoid stack pressure */
unsigned long reg_a;
unsigned long reg_b;

void run_vm(unsigned long start_a, unsigned long start_b) {
    int pc = 0;
    unsigned long steps = 0;
    reg_a = start_a;
    reg_b = start_b;

    while (pc >= 0 && pc < (int)PROG_LEN) {
        Instruction *inst = &program[pc];
        int pc_inc = 1;

        /* Progress indicator for long-running programs */
        if (++steps % 100 == 0) {
            cprintf(".");
        }

        switch (inst->op) {
            case OP_HLF:
                if (inst->reg == REG_A) reg_a /= 2;
                else reg_b /= 2;
                break;
            case OP_TPL:
                if (inst->reg == REG_A) reg_a *= 3;
                else reg_b *= 3;
                break;
            case OP_INC:
                if (inst->reg == REG_A) reg_a++;
                else reg_b++;
                break;
            case OP_JMP:
                pc_inc = inst->offset;
                break;
            case OP_JIE:
                if (inst->reg == REG_A) {
                    if (reg_a % 2 == 0) pc_inc = inst->offset;
                } else {
                    if (reg_b % 2 == 0) pc_inc = inst->offset;
                }
                break;
            case OP_JIO:
                if (inst->reg == REG_A) {
                    if (reg_a == 1) pc_inc = inst->offset;
                } else {
                    if (reg_b == 1) pc_inc = inst->offset;
                }
                break;
        }
        pc += pc_inc;
    }
}

int main(void) {
    /* Set up screen */
    clrscr();
    bgcolor(COLOR_BLUE);
    bordercolor(COLOR_BLACK);
    textcolor(COLOR_WHITE);

    cprintf("ADVENT OF CODE 2015 - DAY 23\r\n");
    cprintf("----------------------------\r\n");
    cprintf("OPENING THE TURING LOCK\r\n\r\n");

    /* Part 1: Initial registers a=0, b=0 */
    cprintf("RUNNING PART 1 (A=0)...\r\n");
    run_vm(0, 0);
    cprintf("RESULT PART 1:\r\n");
    cprintf("REG A: %lu\r\n", reg_a);
    cprintf("REG B: %lu\r\n\r\n", reg_b);

    /* Part 2: Initial registers a=1, b=0 */
    cprintf("RUNNING PART 2 (A=1)...\r\n");
    run_vm(1, 0);
    cprintf("RESULT PART 2:\r\n");
    cprintf("REG A: %lu\r\n", reg_a);
    cprintf("REG B: %lu\r\n\r\n", reg_b);

    cprintf("PRESS ANY KEY TO EXIT.\r\n");
    cgetc();

    return 0;
}
