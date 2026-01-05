#include <conio.h>
#include <stdio.h>
#include <string.h>

// Test inputs for Part 1
#define PART1_TEST_COUNT 9
const char* part1_tests[PART1_TEST_COUNT] = {
    "(())",      // floor 0
    "()()",      // floor 0
    "(((",       // floor 3
    "(()(()(",   // floor 3
    "))(((((",   // floor 3
    "())",       // floor -1
    "))(",       // floor -1
    ")))",       // floor -3
    ")())())"    // floor -3
};

const int part1_expected[PART1_TEST_COUNT] = {0, 0, 3, 3, 3, -1, -1, -3, -3};

// Test inputs for Part 2
#define PART2_TEST_COUNT 2
const char* part2_tests[PART2_TEST_COUNT] = {
    ")",          // basement at position 1
    "()())"       // basement at position 5
};

const int part2_expected[PART2_TEST_COUNT] = {1, 5};

int calculate_floor(const char* input) {
    int floor = 0;
    const char *p = input;
    while (*p) {
        if (*p == '(') {
            floor++;
        } else if (*p == ')') {
            floor--;
        }
        p++;
    }
    return floor;
}

int find_basement_position(const char* input) {
    int floor = 0;
    int pos = 1;
    const char *p;
    for (p = input; *p; ++p, ++pos) {
        if (*p == '(') {
            floor++;
        } else if (*p == ')') {
            floor--;
        }

        if (floor == -1) {
            return pos;
        }
    }
    return -1; // shouldn't happen with valid inputs
}

void test_part1(void) {
    size_t i;
    int result;

    cprintf("Part 1 Tests:\r\n");
    cprintf("============\r\n");

    for (i = 0; i < PART1_TEST_COUNT; i++) {
        result = calculate_floor(part1_tests[i]);
        cprintf("Input: %s -> Floor: %d", part1_tests[i], result);

        if (result == part1_expected[i]) {
            cprintf(" (PASS)\r\n");
        } else {
            cprintf(" (FAIL - expected %d)\r\n", part1_expected[i]);
        }
    }
    cprintf("\r\n");
}

void test_part2(void) {
    size_t i;
    int result;

    cprintf("Part 2 Tests:\r\n");
    cprintf("============\r\n");

    for (i = 0; i < PART2_TEST_COUNT; i++) {
        result = find_basement_position(part2_tests[i]);
        cprintf("Input: %s -> Position: %d", part2_tests[i], result);

        if (result == part2_expected[i]) {
            cprintf(" (PASS)\r\n");
        } else {
            cprintf(" (FAIL - expected %d)\r\n", part2_expected[i]);
        }
    }
    cprintf("\r\n");
}

int main(void) {
    clrscr();

    cprintf("Advent of Code 2015 - Day 1\r\n");
    cprintf("===========================\r\n\r\n");

    test_part1();
    test_part2();

    cprintf("Press any key to exit...\r\n");
    cgetc();

    return 0;
}
