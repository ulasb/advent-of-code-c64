#include <stdio.h>
#include <string.h>
#include <conio.h>

/*
 * Advent of Code 2016 - Day 23 (C64 Edition)
 * Ported by Antigravity
 */

typedef enum {
    OP_CPY, OP_INC, OP_DEC, OP_JNZ, OP_TGL
} OpType;

typedef struct {
    signed char is_reg;
    long val;
} Arg;

typedef struct {
    OpType op;
    Arg args[2];
} Inst;

Inst original_prog[26];
Inst work_prog[26];
typedef enum { REG_A, REG_B, REG_C, REG_D } RegName;
long regs[4]; // a, b, c, d

void init_program() {
    static const Inst program_data[26] = {
        {OP_CPY, {{1, REG_A}, {1, REG_B}}},      /* 0: cpy a b */
        {OP_DEC, {{1, REG_B}, {0, 0}}},      /* 1: dec b */
        {OP_CPY, {{1, REG_A}, {1, REG_D}}},      /* 2: cpy a d */
        {OP_CPY, {{0, 0}, {1, REG_A}}},      /* 3: cpy 0 a */
        {OP_CPY, {{1, REG_B}, {1, REG_C}}},      /* 4: cpy b c */
        {OP_INC, {{1, REG_A}, {0, 0}}},      /* 5: inc a */
        {OP_DEC, {{1, REG_C}, {0, 0}}},      /* 6: dec c */
        {OP_JNZ, {{1, REG_C}, {0, -2}}},     /* 7: jnz c -2 */
        {OP_DEC, {{1, REG_D}, {0, 0}}},      /* 8: dec d */
        {OP_JNZ, {{1, REG_D}, {0, -5}}},     /* 9: jnz d -5 */
        {OP_DEC, {{1, REG_B}, {0, 0}}},      /* 10: dec b */
        {OP_CPY, {{1, REG_B}, {1, REG_C}}},      /* 11: cpy b c */
        {OP_CPY, {{1, REG_C}, {1, REG_D}}},      /* 12: cpy c d */
        {OP_DEC, {{1, REG_D}, {0, 0}}},      /* 13: dec d */
        {OP_INC, {{1, REG_C}, {0, 0}}},      /* 14: inc c */
        {OP_JNZ, {{1, REG_D}, {0, -2}}},     /* 15: jnz d -2 */
        {OP_TGL, {{1, REG_C}, {0, 0}}},      /* 16: tgl c */
        {OP_CPY, {{0, -16}, {1, REG_C}}},    /* 17: cpy -16 c */
        {OP_JNZ, {{0, 1}, {1, REG_C}}},      /* 18: jnz 1 c */
        {OP_CPY, {{0, 84}, {1, REG_C}}},     /* 19: cpy 84 c */
        {OP_JNZ, {{0, 80}, {1, REG_D}}},     /* 20: jnz 80 d */
        {OP_INC, {{1, REG_A}, {0, 0}}},      /* 21: inc a */
        {OP_INC, {{1, REG_D}, {0, 0}}},      /* 22: inc d */
        {OP_JNZ, {{1, REG_D}, {0, -2}}},     /* 23: jnz d -2 */
        {OP_INC, {{1, REG_C}, {0, 0}}},      /* 24: inc c */
        {OP_JNZ, {{1, REG_C}, {0, -5}}},     /* 25: jnz c -5 */
    };
    memcpy(original_prog, program_data, sizeof(program_data));
}

long get_val(Arg *a) {
    if (a->is_reg) return regs[a->val];
    return a->val;
}

long run_puzzle(long initial_a) {
    int pc = 0;
    Inst *ci;
    Inst *ti;
    int target;
    memcpy(work_prog, original_prog, sizeof(original_prog));
    regs[0] = initial_a;
    regs[1] = 0;
    regs[2] = 0;
    regs[3] = 0;

    while (pc >= 0 && pc < 26) {
        /* Optimization check (MUL) */
        /* Pattern: cpy b c, inc a, dec c, jnz c -2, dec d, jnz d -5 */
        if (pc == 4) {
            if (work_prog[4].op == OP_CPY && work_prog[5].op == OP_INC && 
                work_prog[6].op == OP_DEC && work_prog[7].op == OP_JNZ && 
                work_prog[8].op == OP_DEC && work_prog[9].op == OP_JNZ) {
                
                regs[0] += regs[1] * regs[3];
                regs[2] = 0;
                regs[3] = 0;
                pc = 10;
                continue;
            }
        }
        
        /* Add optimization (dec d, inc c, jnz d -2) */
        if (pc == 13) {
             if (work_prog[13].op == OP_DEC && work_prog[14].op == OP_INC && 
                 work_prog[15].op == OP_JNZ && work_prog[15].args[1].val == -2) {
                 regs[2] += regs[3];
                 regs[3] = 0;
                 pc = 16;
                 continue;
             }
        }

        ci = &work_prog[pc];
        switch (ci->op) {
            case OP_CPY:
                if (ci->args[1].is_reg) {
                    regs[ci->args[1].val] = get_val(&ci->args[0]);
                }
                pc++;
                break;
            case OP_INC:
                if (ci->args[0].is_reg) {
                    regs[ci->args[0].val]++;
                }
                pc++;
                break;
            case OP_DEC:
                if (ci->args[0].is_reg) {
                    regs[ci->args[0].val]--;
                }
                pc++;
                break;
            case OP_JNZ:
                if (get_val(&ci->args[0]) != 0) {
                    pc += (int)get_val(&ci->args[1]);
                } else {
                    pc++;
                }
                break;
            case OP_TGL:
                target = pc + (int)get_val(&ci->args[0]);
                if (target >= 0 && target < 26) {
                    ti = &work_prog[target];
                    /* 1-arg instructions */
                    if (ti->op == OP_INC) ti->op = OP_DEC;
                    else if (ti->op == OP_DEC || ti->op == OP_TGL) ti->op = OP_INC;
                    /* 2-arg instructions */
                    else if (ti->op == OP_JNZ) ti->op = OP_CPY;
                    else if (ti->op == OP_CPY) ti->op = OP_JNZ;
                }
                pc++;
                break;
        }
    }
    return regs[0];
}

int main() {
    clrscr();
    bgcolor(COLOR_BLACK);
    bordercolor(COLOR_BLACK);
    textcolor(COLOR_WHITE);
    
    cprintf("ADVENT OF CODE 2016 - DAY 23\r\n");
    cprintf("----------------------------\r\n");
    
    init_program();
    
    cprintf("PART 1 (A=7): RUNNING...\r\n");
    cprintf("PART 1 RESULT: %ld\r\n\r\n", run_puzzle(7));
    
    cprintf("PART 2 (A=12): RUNNING...\r\n");
    cprintf("PART 2 RESULT: %ld\r\n\r\n", run_puzzle(12));
    
    cprintf("PRESS ENTER TO EXIT.\r\n");
    cgetc();
    
    return 0;
}
