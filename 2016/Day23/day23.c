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
long regs[4]; // 0=a, 1=b, 2=c, 3=d

void init_program() {
    int i;
    for (i = 0; i < 26; ++i) {
        original_prog[i].args[0].is_reg = 0;
        original_prog[i].args[0].val = 0;
        original_prog[i].args[1].is_reg = 0;
        original_prog[i].args[1].val = 0;
    }

    // 0: cpy a b
    original_prog[0].op = OP_CPY; original_prog[0].args[0].is_reg = 1; original_prog[0].args[0].val = 0;
    original_prog[0].args[1].is_reg = 1; original_prog[0].args[1].val = 1;

    // 1: dec b
    original_prog[1].op = OP_DEC; original_prog[1].args[0].is_reg = 1; original_prog[1].args[0].val = 1;

    // 2: cpy a d
    original_prog[2].op = OP_CPY; original_prog[2].args[0].is_reg = 1; original_prog[2].args[0].val = 0;
    original_prog[2].args[1].is_reg = 1; original_prog[2].args[1].val = 3;

    // 3: cpy 0 a
    original_prog[3].op = OP_CPY; original_prog[3].args[0].is_reg = 0; original_prog[3].args[0].val = 0;
    original_prog[3].args[1].is_reg = 1; original_prog[3].args[1].val = 0;

    // 4: cpy b c
    original_prog[4].op = OP_CPY; original_prog[4].args[0].is_reg = 1; original_prog[4].args[0].val = 1;
    original_prog[4].args[1].is_reg = 1; original_prog[4].args[1].val = 2;

    // 5: inc a
    original_prog[5].op = OP_INC; original_prog[5].args[0].is_reg = 1; original_prog[5].args[0].val = 0;

    // 6: dec c
    original_prog[6].op = OP_DEC; original_prog[6].args[0].is_reg = 1; original_prog[6].args[0].val = 2;

    // 7: jnz c -2
    original_prog[7].op = OP_JNZ; original_prog[7].args[0].is_reg = 1; original_prog[7].args[0].val = 2;
    original_prog[7].args[1].is_reg = 0; original_prog[7].args[1].val = -2;

    // 8: dec d
    original_prog[8].op = OP_DEC; original_prog[8].args[0].is_reg = 1; original_prog[8].args[0].val = 3;

    // 9: jnz d -5
    original_prog[9].op = OP_JNZ; original_prog[9].args[0].is_reg = 1; original_prog[9].args[0].val = 3;
    original_prog[9].args[1].is_reg = 0; original_prog[9].args[1].val = -5;

    // 10: dec b
    original_prog[10].op = OP_DEC; original_prog[10].args[0].is_reg = 1; original_prog[10].args[0].val = 1;

    // 11: cpy b c
    original_prog[11].op = OP_CPY; original_prog[11].args[0].is_reg = 1; original_prog[11].args[0].val = 1;
    original_prog[11].args[1].is_reg = 1; original_prog[11].args[1].val = 2;

    // 12: cpy c d
    original_prog[12].op = OP_CPY; original_prog[12].args[0].is_reg = 1; original_prog[12].args[0].val = 2;
    original_prog[12].args[1].is_reg = 1; original_prog[12].args[1].val = 3;

    // 13: dec d
    original_prog[13].op = OP_DEC; original_prog[13].args[0].is_reg = 1; original_prog[13].args[0].val = 3;

    // 14: inc c
    original_prog[14].op = OP_INC; original_prog[14].args[0].is_reg = 1; original_prog[14].args[0].val = 2;

    // 15: jnz d -2
    original_prog[15].op = OP_JNZ; original_prog[15].args[0].is_reg = 1; original_prog[15].args[0].val = 3;
    original_prog[15].args[1].is_reg = 0; original_prog[15].args[1].val = -2;

    // 16: tgl c
    original_prog[16].op = OP_TGL; original_prog[16].args[0].is_reg = 1; original_prog[16].args[0].val = 2;

    // 17: cpy -16 c
    original_prog[17].op = OP_CPY; original_prog[17].args[0].is_reg = 0; original_prog[17].args[0].val = -16;
    original_prog[17].args[1].is_reg = 1; original_prog[17].args[1].val = 2;

    // 18: jnz 1 c
    original_prog[18].op = OP_JNZ; original_prog[18].args[0].is_reg = 0; original_prog[18].args[0].val = 1;
    original_prog[18].args[1].is_reg = 1; original_prog[18].args[1].val = 2;

    // 19: cpy 84 c
    original_prog[19].op = OP_CPY; original_prog[19].args[0].is_reg = 0; original_prog[19].args[0].val = 84;
    original_prog[19].args[1].is_reg = 1; original_prog[19].args[1].val = 2;

    // 20: jnz 80 d
    original_prog[20].op = OP_JNZ; original_prog[20].args[0].is_reg = 0; original_prog[20].args[0].val = 80;
    original_prog[20].args[1].is_reg = 1; original_prog[20].args[1].val = 3;

    // 21: inc a
    original_prog[21].op = OP_INC; original_prog[21].args[0].is_reg = 1; original_prog[21].args[0].val = 0;

    // 22: inc d
    original_prog[22].op = OP_INC; original_prog[22].args[0].is_reg = 1; original_prog[22].args[0].val = 3;

    // 23: jnz d -2
    original_prog[23].op = OP_JNZ; original_prog[23].args[0].is_reg = 1; original_prog[23].args[0].val = 3;
    original_prog[23].args[1].is_reg = 0; original_prog[23].args[1].val = -2;

    // 24: inc c
    original_prog[24].op = OP_INC; original_prog[24].args[0].is_reg = 1; original_prog[24].args[0].val = 2;

    // 25: jnz c -5
    original_prog[25].op = OP_JNZ; original_prog[25].args[0].is_reg = 1; original_prog[25].args[0].val = 2;
    original_prog[25].args[1].is_reg = 0; original_prog[25].args[1].val = -5;
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
