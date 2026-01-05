#include <conio.h>
#include <stdio.h>

// Test inputs for Part 1 (Wrapping Paper)
#define PART1_TEST_COUNT 2
const int part1_tests[PART1_TEST_COUNT][3] = {
    {2, 3, 4},    // 58 square feet
    {1, 1, 10}    // 43 square feet
};

const int part1_expected[PART1_TEST_COUNT] = {58, 43};

// Test inputs for Part 2 (Ribbon)
#define PART2_TEST_COUNT 2
const int part2_tests[PART2_TEST_COUNT][3] = {
    {2, 3, 4},    // 34 feet
    {1, 1, 10}    // 14 feet
};

const int part2_expected[PART2_TEST_COUNT] = {34, 14};

void sort_dimensions(int dims[3]) {
    int temp;
    // Simple bubble sort for 3 elements
    if (dims[0] > dims[1]) { temp = dims[0]; dims[0] = dims[1]; dims[1] = temp; }
    if (dims[1] > dims[2]) { temp = dims[1]; dims[1] = dims[2]; dims[2] = temp; }
    if (dims[0] > dims[1]) { temp = dims[0]; dims[0] = dims[1]; dims[1] = temp; }
}

int calculate_wrapping_paper(int l, int w, int h) {
    int dims[3];
    int surface_area;
    int slack;

    dims[0] = l;
    dims[1] = w;
    dims[2] = h;
    sort_dimensions(dims);

    // Surface area: 2*l*w + 2*w*h + 2*h*l
    surface_area = 2 * l * w + 2 * w * h + 2 * h * l;

    // Slack: area of smallest side
    slack = dims[0] * dims[1];

    return surface_area + slack;
}

int calculate_ribbon(int l, int w, int h) {
    int dims[3];
    int wrap;
    int bow;

    dims[0] = l;
    dims[1] = w;
    dims[2] = h;
    sort_dimensions(dims);

    // Wrap: shortest distance around sides (2 * sum of two smallest dimensions)
    wrap = 2 * (dims[0] + dims[1]);

    // Bow: volume of present
    bow = l * w * h;

    return wrap + bow;
}

void test_part1(void) {
    size_t i;
    int result;

    cprintf("Part 1 Tests (Wrapping Paper):\r\n");
    cprintf("==============================\r\n");

    for (i = 0; i < PART1_TEST_COUNT; i++) {
        result = calculate_wrapping_paper(
            part1_tests[i][0],
            part1_tests[i][1],
            part1_tests[i][2]
        );
        cprintf("Dimensions: %dx%dx%d -> Paper: %d sq ft",
                part1_tests[i][0], part1_tests[i][1], part1_tests[i][2], result);

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

    cprintf("Part 2 Tests (Ribbon):\r\n");
    cprintf("=====================\r\n");

    for (i = 0; i < PART2_TEST_COUNT; i++) {
        result = calculate_ribbon(
            part2_tests[i][0],
            part2_tests[i][1],
            part2_tests[i][2]
        );
        cprintf("Dimensions: %dx%dx%d -> Ribbon: %d ft",
                part2_tests[i][0], part2_tests[i][1], part2_tests[i][2], result);

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

    cprintf("Advent of Code 2015 - Day 2\r\n");
    cprintf("===========================\r\n\r\n");

    test_part1();
    test_part2();

    cprintf("Press any key to exit...\r\n");
    cgetc();

    return 0;
}
