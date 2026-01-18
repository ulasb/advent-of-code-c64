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
    return (a + b > c && a + c > b && b + c > a);
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
    /* Expected results constants */
    const int P1_EXPECTED = 2;
    const int P2_EXPECTED = 6;

    /* Part 1 Sample Data */
    int p1_data[][3] = {
        {5, 10, 25},   /* Invalid */
        {10, 10, 10},  /* Valid */
        {2, 3, 4},     /* Valid */
        {1, 2, 3}      /* Invalid */
    };
    int p1_rows = sizeof(p1_data) / sizeof(p1_data[0]);
    int p1_res;

    /* Part 2 Sample Data:
       The triangles are formed by 3 numbers in each column.
       Column 1: (101, 102, 103) and (201, 202, 203)
       Column 2: (301, 302, 303) and (401, 402, 403)
       Column 3: (501, 502, 503) and (601, 602, 603)
    */
    int p2_data[][3] = {
        {101, 301, 501},
        {102, 302, 502},
        {103, 303, 503},
        {201, 401, 601},
        {202, 402, 602},
        {203, 403, 603}
    };
    int p2_rows = sizeof(p2_data) / sizeof(p2_data[0]);
    int p2_res;

    cprintf("\r\nRUNNING TESTS...\r\n");

    /* Part 1 Test */
    cprintf("PART 1 TEST: ");
    p1_res = solve_part1(p1_data, p1_rows);
    if (p1_res == P1_EXPECTED) {
        cprintf("PASS (%d/%d)\r\n", p1_res, P1_EXPECTED);
    } else {
        cprintf("FAIL (got %d, expected %d)\r\n", p1_res, P1_EXPECTED);
    }

    /* Part 2 Test */
    cprintf("PART 2 TEST: ");
    p2_res = solve_part2(p2_data, p2_rows);
    if (p2_res == P2_EXPECTED) {
        cprintf("PASS (%d/%d)\r\n", p2_res, P2_EXPECTED);
    } else {
        cprintf("FAIL (got %d, expected %d)\r\n", p2_res, P2_EXPECTED);
    }
}
