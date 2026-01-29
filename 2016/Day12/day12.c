/*
 * Advent of Code 2016 - Day 12
 * Leonardo's Monorail
 *
 * C64 version using cc65.
 */

#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef enum { CPY, INC, DEC, JNZ } Op;

typedef struct {
    Op op;
    signed char arg1_is_reg;
    int arg1_val;
    signed char arg2_is_reg;
    int arg2_val;
} Instr;

long regs[4];

void reset_regs(long a, long b, long c, long d) {
    regs[0] = a;
    regs[1] = b;
    regs[2] = c;
    regs[3] = d;
}

void run_program(Instr* instrs, int n) {
    int pc = 0;
    while (pc < n) {
        Instr* i = &instrs[pc];

        /* Peephole Optimization: Addition Loop
         * Pattern 1: inc x, dec y, jnz y -2  =>  x += y, y = 0
         * Pattern 2: dec y, inc x, jnz y -2  =>  x += y, y = 0
         */
        if (pc + 2 < n) {
            Instr* i2 = &instrs[pc + 1];
            Instr* i3 = &instrs[pc + 2];
            if (i3->op == JNZ && !i3->arg2_is_reg && i3->arg2_val == -2) {
                if (i->op == INC && i2->op == DEC && i3->arg1_is_reg && i3->arg1_val == i2->arg1_val) {
                    regs[i->arg1_val] += regs[i2->arg1_val];
                    regs[i2->arg1_val] = 0;
                    pc += 3;
                    continue;
                } else if (i->op == DEC && i2->op == INC && i3->arg1_is_reg && i3->arg1_val == i->arg1_val) {
                    regs[i2->arg1_val] += regs[i->arg1_val];
                    regs[i->arg1_val] = 0;
                    pc += 3;
                    continue;
                }
            }
        }

        switch (i->op) {
            case CPY:
                regs[i->arg2_val] = i->arg1_is_reg ? regs[i->arg1_val] : (long)i->arg1_val;
                pc++;
                break;
            case INC:
                regs[i->arg1_val]++;
                pc++;
                break;
            case DEC:
                regs[i->arg1_val]--;
                pc++;
                break;
            case JNZ:
                {
                    long val = i->arg1_is_reg ? regs[i->arg1_val] : (long)i->arg1_val;
                    if (val != 0) {
                        pc += i->arg2_val;
                    } else {
                        pc++;
                    }
                }
                break;
        }
    }
}

/* Input Instructions */
Instr input_instrs[] = {
    {CPY, 0, 1, 1, 0},   /* cpy 1 a */
    {CPY, 0, 1, 1, 1},   /* cpy 1 b */
    {CPY, 0, 26, 1, 3},  /* cpy 26 d */
    {JNZ, 1, 2, 0, 2},   /* jnz c 2 */
    {JNZ, 0, 1, 0, 5},   /* jnz 1 5 */
    {CPY, 0, 7, 1, 2},   /* cpy 7 c */
    {INC, 1, 3, 0, 0},   /* inc d */
    {DEC, 1, 2, 0, 0},   /* dec c */
    {JNZ, 1, 2, 0, -2},  /* jnz c -2 */
    {CPY, 1, 0, 1, 2},   /* cpy a c */
    {INC, 1, 0, 0, 0},   /* inc a */
    {DEC, 1, 1, 0, 0},   /* dec b */
    {JNZ, 1, 1, 0, -2},  /* jnz b -2 */
    {CPY, 1, 2, 1, 1},   /* cpy c b */
    {DEC, 1, 3, 0, 0},   /* dec d */
    {JNZ, 1, 3, 0, -6},  /* jnz d -6 */
    {CPY, 0, 19, 1, 2},  /* cpy 19 c */
    {CPY, 0, 11, 1, 3},  /* cpy 11 d */
    {INC, 1, 0, 0, 0},   /* inc a */
    {DEC, 1, 3, 0, 0},   /* dec d */
    {JNZ, 1, 3, 0, -2},  /* jnz d -2 */
    {DEC, 1, 2, 0, 0},   /* dec c */
    {JNZ, 1, 2, 0, -5}   /* jnz c -5 */
};
int num_input_instrs = sizeof(input_instrs) / sizeof(Instr);

void run_tests() {
    /* Test Example */
    Instr test_instrs[] = {
        {CPY, 0, 41, 1, 0},  /* cpy 41 a */
        {INC, 1, 0, 0, 0},   /* inc a */
        {INC, 1, 0, 0, 0},   /* inc a */
        {DEC, 1, 0, 0, 0},   /* dec a */
        {JNZ, 1, 0, 0, 2},   /* jnz a 2 */
        {DEC, 1, 0, 0, 0}    /* dec a */
    };
    cprintf("Running unit tests...\r\n");
    reset_regs(0, 0, 0, 0);
    run_program(test_instrs, 6);
    if (regs[0] == 42) cprintf("Test Example: PASS\r\n");
    else cprintf("Test Example: FAIL (expected 42, got %ld)\r\n", regs[0]);

    /* Test cpy register */
    {
        Instr t[] = { {CPY, 0, 10, 1, 0}, {CPY, 1, 0, 1, 1} };
        reset_regs(0, 0, 0, 0);
        run_program(t, 2);
        if (regs[1] == 10) cprintf("Test cpy reg: PASS\r\n");
        else cprintf("Test cpy reg: FAIL\r\n");
    }

    /* Test jnz zero */
    {
        Instr t[] = { {JNZ, 0, 0, 0, 2}, {INC, 1, 0, 0, 0}, {INC, 1, 0, 0, 0} };
        reset_regs(0, 0, 0, 0);
        run_program(t, 3);
        if (regs[0] == 2) cprintf("Test jnz zero: PASS\r\n");
        else cprintf("Test jnz zero: FAIL\r\n");
    }

    /* Test jnz nonzero */
    {
        Instr t[] = { {JNZ, 0, 1, 0, 2}, {INC, 1, 0, 0, 0}, {INC, 1, 0, 0, 0} };
        reset_regs(0, 0, 0, 0);
        run_program(t, 3);
        if (regs[0] == 1) cprintf("Test jnz nonzero: PASS\r\n");
        else cprintf("Test jnz nonzero: FAIL\r\n");
    }

    /* Test peephole add */
    {
        Instr t[] = { {CPY, 0, 5, 1, 1}, {CPY, 0, 10, 1, 0}, {INC, 1, 0, 0, 0}, {DEC, 1, 1, 0, 0}, {JNZ, 1, 1, 0, -2} };
        reset_regs(0, 0, 0, 0);
        run_program(t, 5);
        if (regs[0] == 15 && regs[1] == 0) cprintf("Test peephole: PASS\r\n");
        else cprintf("Test peephole: FAIL (%ld, %ld)\r\n", regs[0], regs[1]);
    }
    cprintf("Tests complete.\r\n\r\n");
}

int main() {
    clock_t start, end;

    clrscr();
    bgcolor(COLOR_BLUE);
    bordercolor(COLOR_LIGHTBLUE);
    textcolor(COLOR_WHITE);

    run_tests();

    cprintf("Starting Part 1...\r\n");
    reset_regs(0, 0, 0, 0);
    start = clock();
    run_program(input_instrs, num_input_instrs);
    end = clock();
    cprintf("[Part 1] Register a: %ld\r\n", regs[0]);
    cprintf("Ticks: %ld\r\n\r\n", (long)(end - start));

    cprintf("Starting Part 2...\r\n");
    reset_regs(0, 0, 1, 0);
    start = clock();
    run_program(input_instrs, num_input_instrs);
    end = clock();
    cprintf("[Part 2] Register a: %ld\r\n", regs[0]);
    cprintf("Ticks: %ld\r\n", (long)(end - start));

    cprintf("\r\nPRESS ANY KEY TO EXIT.\r\n");
    cgetc();

    return 0;
}
