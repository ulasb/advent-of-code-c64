#include <stdio.h>
#include <conio.h>

/*
 * Advent of Code 2016 - Day 15 (C64 Edition)
 * 
 * This program calculates the first time step to press a button so that a capsule
 * falls through a series of rotating discs.
 */

typedef struct {
    unsigned int num_positions;
    unsigned int starting_position;
    unsigned int index;
} Disc;

/* Calculate Greatest Common Divisor */
unsigned long gcd(unsigned long a, unsigned long b) {
    while (b) {
        unsigned long tmp;
        a %= b;
        tmp = a;
        a = b;
        b = tmp;
    }
    return a;
}

/* Calculate Least Common Multiple */
unsigned long lcm(unsigned long a, unsigned long b) {
    if (a == 0 || b == 0) return 0;
    /* Divide first to avoid overflow */
    return (a / gcd(a, b)) * b;
}

/* 
 * Find the earliest time to push the button.
 * The condition for each disc is: (starting_position + time + index) % num_positions == 0
 */
unsigned long solve(Disc* discs, int count) {
    unsigned long time = 0;
    unsigned long step = 1;
    int i;

    for (i = 0; i < count; ++i) {
        /* Increment time by step until current disc is at position 0 when capsule arrives */
        while ((unsigned long)(discs[i].starting_position + time + discs[i].index) % discs[i].num_positions != 0) {
            time += step;
        }
        /* Update step to the LCM of current step and the disc's positions */
        step = lcm(step, (unsigned long)discs[i].num_positions);
    }
    return time;
}

void run_test_case(const char* name, Disc* discs, int count, unsigned long expected, int check) {
    unsigned long result;
    cprintf("TEST: %s\r\n", name);
    result = solve(discs, count);
    cprintf("RESULT: %lu\r\n", result);
    if (check) {
        if (result == expected) {
            textcolor(COLOR_GREEN);
            cprintf("STATUS: PASS\r\n");
        } else {
            textcolor(COLOR_RED);
            cprintf("STATUS: FAIL (EXPECTED %lu)\r\n", expected);
        }
        textcolor(COLOR_WHITE);
    }
    cprintf("\r\n");
}

int main(void) {
    /* Test cases */
    Disc example[] = {
        {5, 4, 1},
        {2, 1, 2}
    };

    Disc part1[] = {
        {13, 1, 1},
        {19, 10, 2},
        {3, 2, 3},
        {7, 1, 4},
        {5, 3, 5},
        {17, 5, 6}
    };

    Disc part2[] = {
        {13, 1, 1},
        {19, 10, 2},
        {3, 2, 3},
        {7, 1, 4},
        {5, 3, 5},
        {17, 5, 6},
        {11, 0, 7}
    };

    clrscr();
    bgcolor(COLOR_BLACK);
    bordercolor(COLOR_BLACK);
    textcolor(COLOR_WHITE);

    cprintf("ADVENT OF CODE 2016 - DAY 15\r\n");
    cprintf("----------------------------\r\n\r\n");

    run_test_case("EXAMPLE", example, 2, 5, 1);
    run_test_case("PART 1", part1, 6, 0, 0);
    run_test_case("PART 2", part2, 7, 0, 0);

    cprintf("PRESS ANY KEY TO EXIT.\r\n");
    cgetc();

    return 0;
}
