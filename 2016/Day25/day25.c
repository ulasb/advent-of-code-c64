#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Advent of Code 2016 - Day 25: Clock Signal
 * C64 Edition
 * 
 * This program implements an Assembunny interpreter specialized for Day 25.
 * It searches for the smallest non-negative integer for register 'a' that
 * produces an alternating 0, 1, 0, 1... clock signal.
 */

#define MAX_INSTR 32
#define MAX_CHECK 30  /* Number of outputs to check for validity */

typedef enum {
    OP_CPY, OP_INC, OP_DEC, OP_JNZ, OP_OUT
} Opcode;

typedef struct {
    Opcode op;
    signed char arg1_reg; /* -1 if literal, 0-3 for a-d */
    int arg1_val;
    signed char arg2_reg;
    int arg2_val;
} Instruction;

Instruction program[MAX_INSTR];
int num_instr = 0;
long regs[4]; /* a, b, c, d */

void add_instr(Opcode op, signed char a1r, int a1v, signed char a2r, int a2v) {
    if (num_instr < MAX_INSTR) {
        program[num_instr].op = op;
        program[num_instr].arg1_reg = a1r;
        program[num_instr].arg1_val = a1v;
        program[num_instr].arg2_reg = a2r;
        program[num_instr].arg2_val = a2v;
        num_instr++;
    }
}

/* Hardcoded test case (representative of AoC 2016 Day 25 input) */
void load_program() {
    num_instr = 0;
    
    add_instr(OP_CPY, 0, 0, 1, 0);   /* 0: cpy a b */
    add_instr(OP_CPY, -1, 7, 2, 0);  /* 1: cpy 7 c */
    add_instr(OP_CPY, -1, 362, 3, 0); /* 2: cpy 362 d */
    add_instr(OP_INC, 1, 0, -1, 0);   /* 3: inc b --- MUL start --- */
    add_instr(OP_DEC, 3, 0, -1, 0);   /* 4: dec d */
    add_instr(OP_JNZ, 3, 0, -1, -2);  /* 5: jnz d -2 */
    add_instr(OP_DEC, 2, 0, -1, 0);   /* 6: dec c */
    add_instr(OP_JNZ, 2, 0, -1, -5);  /* 7: jnz c -5 --- MUL end --- */
    add_instr(OP_CPY, 1, 0, 0, 0);   /* 8: cpy b a */
    add_instr(OP_JNZ, -1, 0, -1, 0);  /* 9: jnz 0 0 */
    add_instr(OP_CPY, 0, 0, 1, 0);   /* 10: cpy a b */
    add_instr(OP_CPY, -1, 0, 0, 0);  /* 11: cpy 0 a */
    add_instr(OP_CPY, -1, 2, 2, 0);  /* 12: cpy 2 c */
    add_instr(OP_JNZ, 1, 0, -1, 2);   /* 13: jnz b 2 */
    add_instr(OP_JNZ, -1, 1, -1, 6);  /* 14: jnz 1 6 */
    add_instr(OP_DEC, 1, 0, -1, 0);   /* 15: dec b */
    add_instr(OP_DEC, 2, 0, -1, 0);   /* 16: dec c */
    add_instr(OP_JNZ, 2, 0, -1, -4);  /* 17: jnz c -4 */
    add_instr(OP_INC, 0, 0, -1, 0);   /* 18: inc a */
    add_instr(OP_JNZ, -1, 1, -1, -7); /* 19: jnz 1 -7 */
    add_instr(OP_CPY, -1, 2, 1, 0);  /* 20: cpy 2 b */
    add_instr(OP_JNZ, 2, 0, -1, 2);   /* 21: jnz c 2 */
    add_instr(OP_JNZ, -1, 1, -1, 4);  /* 22: jnz 1 4 */
    add_instr(OP_DEC, 1, 0, -1, 0);   /* 23: dec b */
    add_instr(OP_DEC, 2, 0, -1, 0);   /* 24: dec c */
    add_instr(OP_JNZ, 2, 0, -1, -4);  /* 25: jnz c -4 */
    add_instr(OP_JNZ, -1, 0, -1, 0);  /* 26: jnz 0 0 */
    add_instr(OP_OUT, 1, 0, -1, 0);   /* 27: out b */
    add_instr(OP_JNZ, 0, 0, -1, -18); /* 28: jnz a -18 */
    add_instr(OP_JNZ, -1, 1, -1, -21); /* 29: jnz 1 -21 */
}

int run_with_a(long initial_a) {
    int pc = 0;
    int next_expected = 0;
    int outputs_found = 0;
    int safety_counter = 0;
    Instruction* instr;
    
    regs[0] = initial_a;
    regs[1] = 0;
    regs[2] = 0;
    regs[3] = 0;
    
    while (pc < num_instr && outputs_found < MAX_CHECK && safety_counter < 10000) {
        instr = &program[pc];
        safety_counter++;
        
        /* Simple optimization: detect MULTIPLICATION idiom at pc=3 */
        /* Pattern: inc b, dec d, jnz d -2, dec c, jnz c -5 */
        if (pc == 3) {
            regs[1] += regs[3] * regs[2];
            regs[3] = 0;
            regs[2] = 0;
            pc = 8;
            continue;
        }

        /* Simple optimization: detect ADDITION idiom at pc=15 */
        /* Pattern: dec b, dec c, jnz c -4, inc a... (part of div2) */
        /* We'll skip complex div2 optimization for now to keep it simple, 
           but the MUL optimization above is the most critical one. */

        switch (instr->op) {
            case OP_CPY:
                if (instr->arg2_reg != -1) {
                    regs[instr->arg2_reg] = (instr->arg1_reg == -1) ? instr->arg1_val : regs[instr->arg1_reg];
                }
                pc++;
                break;
            case OP_INC:
                if (instr->arg1_reg != -1) regs[instr->arg1_reg]++;
                pc++;
                break;
            case OP_DEC:
                if (instr->arg1_reg != -1) regs[instr->arg1_reg]--;
                pc++;
                break;
            case OP_JNZ:
                if (((instr->arg1_reg == -1) ? instr->arg1_val : regs[instr->arg1_reg]) != 0) {
                    pc += (instr->arg2_reg == -1) ? instr->arg2_val : regs[instr->arg2_reg];
                } else {
                    pc++;
                }
                break;
            case OP_OUT: {
                long val = (instr->arg1_reg == -1) ? instr->arg1_val : regs[instr->arg1_reg];
                if (val != next_expected) return 0;
                next_expected = 1 - next_expected;
                outputs_found++;
                pc++;
                break;
            }
        }
    }
    
    return (outputs_found >= MAX_CHECK);
}

int main(void) {
    long a = 0;
    
    /* Set up C64 screen colors */
    bgcolor(COLOR_BLUE);
    bordercolor(COLOR_LIGHTBLUE);
    textcolor(COLOR_WHITE);
    clrscr();
    
    cprintf("ADVENT OF CODE 2016 - DAY 25\r\n");
    cprintf("CLOCK SIGNAL\r\n");
    cprintf("============================\r\n\r\n");
    
    cprintf("LOADING PROGRAM...\r\n");
    load_program();
    
    cprintf("SEARCHING FOR LOWEST 'A'...\r\n");
    
    while (a < 1000) {
        if (a % 20 == 0) {
            gotoxy(0, 7);
            cprintf("TESTING A: %ld  ", a);
        }
        
        if (run_with_a(a)) {
            gotoxy(0, 9);
            textcolor(COLOR_LIGHTGREEN);
            cprintf("RESULT FOUND! A = %ld\r\n", a);
            textcolor(COLOR_WHITE);
            break;
        }
        a++;
    }
    
    if (a >= 1000) {
        cprintf("\r\nNO RESULT FOUND UP TO 1000.\r\n");
    }
    
    cprintf("\r\nDONE.\r\n");
    cprintf("PRESS ANY KEY TO EXIT.\r\n");
    cgetc();
    
    return 0;
}
