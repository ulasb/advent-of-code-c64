#include <conio.h>
#include <stdio.h>
#include <string.h>

// Grid size for tracking visited houses (11x11 centered on origin)
#define GRID_SIZE 11
#define OFFSET 5  // (0,0) maps to [5][5]

// Test inputs for Part 1
#define PART1_TEST_COUNT 3
const char* part1_tests[PART1_TEST_COUNT] = {
    ">",           // 2 houses: start + east
    "^>v<",        // 4 houses: square path
    "^v^v^v^v^v"   // 2 houses: alternating up/down
};

const int part1_expected[PART1_TEST_COUNT] = {2, 4, 2};

// Test inputs for Part 2
#define PART2_TEST_COUNT 3
const char* part2_tests[PART2_TEST_COUNT] = {
    "^v",          // 3 houses: Santa north, Robot Santa south
    "^>v<",        // 3 houses: alternating path
    "^v^v^v^v^v"   // 11 houses: Santa up, Robot Santa down
};

const int part2_expected[PART2_TEST_COUNT] = {3, 3, 11};

void clear_visited_grid(char visited[GRID_SIZE][GRID_SIZE]) {
    memset(visited, 0, sizeof(char) * GRID_SIZE * GRID_SIZE);
}

int visit_if_new(char visited[GRID_SIZE][GRID_SIZE], int x, int y) {
    if (x + OFFSET >= 0 && x + OFFSET < GRID_SIZE &&
        y + OFFSET >= 0 && y + OFFSET < GRID_SIZE) {
        if (visited[y + OFFSET][x + OFFSET] == 0) {
            visited[y + OFFSET][x + OFFSET] = 1;
            return 1;
        }
    }
    return 0;
}

int track_santa_visits(const char* directions) {
    char visited[GRID_SIZE][GRID_SIZE];
    int x, y;
    int unique_houses;
    const char* p;

    clear_visited_grid(visited);

    x = 0;
    y = 0;
    unique_houses = 1;  // Start with the initial house

    // Mark starting position as visited
    visited[y + OFFSET][x + OFFSET] = 1;

    // Process each direction
    p = directions;
    while (*p) {
        switch (*p) {
            case '^': y++; break;  // north
            case 'v': y--; break;  // south
            case '>': x++; break;  // east
            case '<': x--; break;  // west
        }

        // Check bounds (shouldn't exceed grid in test cases)
        if (visit_if_new(visited, x, y)) {
            unique_houses++;
        }

        p++;
    }

    return unique_houses;
}

int track_santa_and_robot_santa(const char* directions) {
    char visited[GRID_SIZE][GRID_SIZE];
    int santa_x, santa_y;
    int robot_x, robot_y;
    int unique_houses;
    const char* p;
    int turn;

    clear_visited_grid(visited);

    santa_x = 0;
    santa_y = 0;
    robot_x = 0;
    robot_y = 0;
    unique_houses = 1;  // Start with the initial house

    // Mark starting position as visited
    visited[0 + OFFSET][0 + OFFSET] = 1;

    // Process each direction, alternating between Santa and Robot Santa
    p = directions;
    turn = 0;  // 0 = Santa, 1 = Robot Santa

    while (*p) {
        int* curr_x = (turn == 0) ? &santa_x : &robot_x;
        int* curr_y = (turn == 0) ? &santa_y : &robot_y;

        switch (*p) {
            case '^': (*curr_y)++; break;  // north
            case 'v': (*curr_y)--; break;  // south
            case '>': (*curr_x)++; break;  // east
            case '<': (*curr_x)--; break;  // west
        }

        // Check bounds and mark as visited if new
        if (visit_if_new(visited, *curr_x, *curr_y)) {
            unique_houses++;
        }

        turn = 1 - turn;  // Switch turns
        p++;
    }

    return unique_houses;
}

void test_part1(void) {
    size_t i;
    int result;

    cprintf("Part 1 Tests:\r\n");
    cprintf("============\r\n");

    for (i = 0; i < sizeof(part1_tests) / sizeof(part1_tests[0]); i++) {
        result = track_santa_visits(part1_tests[i]);
        cprintf("Input: %s -> Houses: %d", part1_tests[i], result);

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

    for (i = 0; i < sizeof(part2_tests) / sizeof(part2_tests[0]); i++) {
        result = track_santa_and_robot_santa(part2_tests[i]);
        cprintf("Input: %s -> Houses: %d", part2_tests[i], result);

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

    cprintf("Advent of Code 2015 - Day 3\r\n");
    cprintf("===========================\r\n\r\n");

    test_part1();
    test_part2();

    cprintf("Press any key to exit...\r\n");
    cgetc();

    return 0;
}
