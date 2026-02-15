#include <stdio.h>
#include <conio.h>

/**
 * Advent of Code 2016 - Day 19: An Elf House Divided
 * Commodore 64 implementation using cc65.
 */

/* The problem input */
#define INPUT_N 3012210UL

/**
 * Part 1: Standard Josephus problem (skip 1).
 * Formula: J(n) = 2(n - 2^floor(log2(n))) + 1
 */
unsigned long solve_part1(unsigned long n) {
    unsigned long p = 1;
    while (p * 2 <= n) {
        p *= 2;
    }
    return 2 * (n - p) + 1;
}

/**
 * Part 2: Josephus variant (remove from across the circle).
 * Let p be the largest power of 3 <= n.
 * If n = p, winner is p.
 * If n <= 2p, winner is n - p.
 * If n > 2p, winner is 2n - 3p.
 */
unsigned long solve_part2(unsigned long n) {
    unsigned long p = 1;
    while (p * 3 <= n) {
        p *= 3;
    }
    
    if (n == p) {
        return n;
    } else if (n <= 2 * p) {
        return n - p;
    } else {
        return (2 * n) - (3 * p);
    }
}

int main(void) {
    unsigned long res1, res2;

    clrscr();
    textcolor(COLOR_WHITE);
    cprintf("aoc 2016 - day 19\r\n");
    cprintf("-----------------\r\n");
    cprintf("input n: %lu\r\n\r\n", INPUT_N);

    cprintf("calculating part 1...\r\n");
    res1 = solve_part1(INPUT_N);
    cprintf("part 1: %lu\r\n\r\n", res1);

    cprintf("calculating part 2...\r\n");
    res2 = solve_part2(INPUT_N);
    cprintf("part 2: %lu\r\n\r\n", res2);

    cprintf("press any key to exit.");
    cgetc();

    return 0;
}
