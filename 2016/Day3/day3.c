#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

/* Function Prototypes */
int is_valid_triangle(int a, int b, int c);
int solve_part1(int data[][3], int rows);
int solve_part2(int data[][3], int rows);
void run_tests(void);

int main(void) {
    bgcolor(COLOR_BLUE);
    bordercolor(COLOR_LIGHTBLUE);
    textcolor(COLOR_WHITE);
    clrscr();
    gotoxy(0, 0);

    cprintf("ADVENT OF CODE 2016 - DAY 3\r\n");
    cprintf("SQUARES WITH THREE SIDES\r\n");
    cprintf("============================\r\n");

    run_tests();

    cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
    cgetc();

    return 0;
}

int is_valid_triangle(int a, int b, int c) {
    /* Triangle Inequality Theorem: 
       Sum of any two sides must be greater than the third side. */
    if (a + b > c && a + c > b && b + c > a) {
        return 1;
    }
    return 0;
}

int solve_part1(int data[][3], int rows) {
    int count = 0;
    int i;
    for (i = 0; i < rows; i++) {
        if (is_valid_triangle(data[i][0], data[i][1], data[i][2])) {
            count++;
        }
    }
    return count;
}

int solve_part2(int data[][3], int rows) {
    int count = 0;
    int i, j;
    /* Process in blocks of 3 rows */
    for (i = 0; i < rows; i += 3) {
        /* Each column in the 3-row block forms a triangle */
        for (j = 0; j < 3; j++) {
            if (is_valid_triangle(data[i][j], data[i+1][j], data[i+2][j])) {
                count++;
            }
        }
    }
    return count;
}

void run_tests(void) {
    /* Part 1 Sample Data */
    int p1_data[4][3] = {
        {5, 10, 25},   /* Invalid */
        {10, 10, 10},  /* Valid */
        {2, 3, 4},     /* Valid */
        {1, 2, 3}      /* Invalid */
    };
    int p1_res;

    /* Part 2 Sample Data */
    int p2_data[6][3] = {
        {101, 301, 501},
        {102, 302, 502},
        {103, 303, 503},
        {201, 401, 601},
        {202, 402, 602},
        {203, 403, 603}
    };
    int p2_res;

    cprintf("\r\nRUNNING TESTS...\r\n");

    /* Part 1 Test */
    cprintf("PART 1 TEST: ");
    p1_res = solve_part1(p1_data, 4);
    if (p1_res == 2) {
        cprintf("PASS (%d/2)\r\n", p1_res);
    } else {
        cprintf("FAIL (got %d, expected 2)\r\n", p1_res);
    }

    /* Part 2 Test */
    cprintf("PART 2 TEST: ");
    p2_res = solve_part2(p2_data, 6);
    if (p2_res == 6) {
        cprintf("PASS (%d/6)\r\n", p2_res);
    } else {
        cprintf("FAIL (got %d, expected 6)\r\n", p2_res);
    }
}
