#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants for demonstration grid (50x50 fits in C64 memory ~2.5KB)
#define DEMO_ROWS 50
#define DEMO_COLS 50

// Global grids to avoid cc65 local variable limits
char part1_grid[DEMO_ROWS][DEMO_COLS];
unsigned int part2_grid[DEMO_ROWS][DEMO_COLS];

// Test instructions for Part 1 (scaled for 50x50 demo grid)
#define PART1_TEST_COUNT 3
const char* part1_instructions[PART1_TEST_COUNT] = {
    "turn on 0,0 through 49,49",     // Turn on all lights in 50x50 grid
    "toggle 0,0 through 49,0",       // Toggle first row (50 lights)
    "turn off 24,24 through 25,25"   // Turn off 2x2 square in middle (4 lights)
};

const unsigned long part1_expected[PART1_TEST_COUNT] = {
    2500,  // All 50x50 grid on: 50 * 50 = 2,500
    2450,  // First row toggled off: 2,500 - 50 = 2,450
    2446   // Middle 4 turned off: 2,450 - 4 = 2,446
};

// Test instructions for Part 2 (scaled for 50x50 demo grid)
#define PART2_TEST_COUNT 2
const char* part2_instructions[PART2_TEST_COUNT] = {
    "turn on 0,0 through 0,0",     // Single light +1 brightness
    "toggle 0,0 through 49,49"     // All lights toggle +2 each
};

const unsigned long part2_expected[PART2_TEST_COUNT] = {
    1,      // Single light brightness +1
    5001    // Total after toggle: 1 + (2,500 * 2) = 5,001
};

// Command types for efficient processing
typedef enum {
    CMD_TURN_ON,
    CMD_TURN_OFF,
    CMD_TOGGLE
} CommandType;

// Structure to represent a light command
typedef struct {
    CommandType cmd_type;  // Efficient enum instead of string
    int start_x, start_y;
    int end_x, end_y;
} LightCommand;

// Simple atoi for C64 (no stdlib dependency)
int simple_atoi(const char* str) {
    int result = 0;
    int i = 0;
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return result;
}

// Parse a single instruction line (C64-compatible)
void parse_instruction(const char* line, LightCommand* cmd) {
    int pos = 0;
    char num_str[10];
    int num_pos;

    // Parse command type
    if (line[0] == 't' && line[1] == 'u' && line[2] == 'r' && line[3] == 'n') {
        // "turn on/off"
        pos = 5; // skip "turn "
        if (line[pos] == 'o' && line[pos+1] == 'n') {
            cmd->cmd_type = CMD_TURN_ON;
            pos += 3; // skip "on "
        } else {
            cmd->cmd_type = CMD_TURN_OFF;
            pos += 4; // skip "off "
        }
    } else {
        // "toggle"
        cmd->cmd_type = CMD_TOGGLE;
        pos = 7; // skip "toggle "
    }

    // Parse start_x
    num_pos = 0;
    while (line[pos] >= '0' && line[pos] <= '9') {
        num_str[num_pos++] = line[pos++];
    }
    num_str[num_pos] = '\0';
    cmd->start_x = simple_atoi(num_str);
    pos++; // skip comma

    // Parse start_y
    num_pos = 0;
    while (line[pos] >= '0' && line[pos] <= '9') {
        num_str[num_pos++] = line[pos++];
    }
    num_str[num_pos] = '\0';
    cmd->start_y = simple_atoi(num_str);

    // Skip " through "
    pos += 9; // skip " through "

    // Parse end_x
    num_pos = 0;
    while (line[pos] >= '0' && line[pos] <= '9') {
        num_str[num_pos++] = line[pos++];
    }
    num_str[num_pos] = '\0';
    cmd->end_x = simple_atoi(num_str);
    pos++; // skip comma

    // Parse end_y
    num_pos = 0;
    while (line[pos] >= '0' && line[pos] <= '9') {
        num_str[num_pos++] = line[pos++];
    }
    num_str[num_pos] = '\0';
    cmd->end_y = simple_atoi(num_str);
}

// Process a single command on a grid with incremental counting
void process_command_part1(char grid[DEMO_ROWS][DEMO_COLS], const LightCommand* cmd, unsigned long* light_count) {
    int x, y;
    int start_x = cmd->start_x;
    int start_y = cmd->start_y;
    int end_x = cmd->end_x;
    int end_y = cmd->end_y;

    // Clamp coordinates to grid bounds for demo
    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;
    if (end_x >= DEMO_ROWS) end_x = DEMO_ROWS - 1;
    if (end_y >= DEMO_COLS) end_y = DEMO_COLS - 1;

    for (x = start_x; x <= end_x; x++) {
        for (y = start_y; y <= end_y; y++) {
            switch (cmd->cmd_type) {
                case CMD_TURN_ON:
                    if (grid[x][y] == 0) {
                        grid[x][y] = 1;
                        (*light_count)++;
                    }
                    break;
                case CMD_TURN_OFF:
                    if (grid[x][y] == 1) {
                        grid[x][y] = 0;
                        (*light_count)--;
                    }
                    break;
                case CMD_TOGGLE:
                    if (grid[x][y] == 1) {
                        grid[x][y] = 0;
                        (*light_count)--;
                    } else {
                        grid[x][y] = 1;
                        (*light_count)++;
                    }
                    break;
            }
        }
    }
}

// Simulate Part 1 with actual grid operations (scaled for C64)
unsigned long simulate_part1_full(const LightCommand* commands, int num_commands) {
    int x, y, i;
    unsigned long light_count = 0;

    // Initialize grid to off
    for (x = 0; x < DEMO_ROWS; x++) {
        for (y = 0; y < DEMO_COLS; y++) {
            part1_grid[x][y] = 0;
        }
    }

    // Process each command and show intermediate results
    for (i = 0; i < num_commands; i++) {
        process_command_part1(part1_grid, &commands[i], &light_count);
        cprintf("Step %d: %lu lights\r\n", i+1, light_count);
    }

    return light_count;
}

// Process a single command on brightness grid with incremental counting (Part 2)
void process_command_part2(unsigned int grid[DEMO_ROWS][DEMO_COLS], const LightCommand* cmd, unsigned long* brightness_total) {
    int x, y;
    int start_x = cmd->start_x;
    int start_y = cmd->start_y;
    int end_x = cmd->end_x;
    int end_y = cmd->end_y;

    // Clamp coordinates to grid bounds for demo
    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;
    if (end_x >= DEMO_ROWS) end_x = DEMO_ROWS - 1;
    if (end_y >= DEMO_COLS) end_y = DEMO_COLS - 1;

    for (x = start_x; x <= end_x; x++) {
        for (y = start_y; y <= end_y; y++) {
            switch (cmd->cmd_type) {
                case CMD_TURN_ON:
                    grid[x][y] += 1;
                    (*brightness_total) += 1;
                    break;
                case CMD_TURN_OFF:
                    if (grid[x][y] > 0) {
                        grid[x][y] -= 1;
                        (*brightness_total) -= 1;
                    }
                    break;
                case CMD_TOGGLE:
                    grid[x][y] += 2;
                    (*brightness_total) += 2;
                    break;
            }
        }
    }
}

// Simulate Part 2 with actual grid operations (scaled for C64)
unsigned long simulate_part2_full(const LightCommand* commands, int num_commands) {
    int x, y, i;
    unsigned long brightness_total = 0;

    // Initialize brightness grid to 0
    for (x = 0; x < DEMO_ROWS; x++) {
        for (y = 0; y < DEMO_COLS; y++) {
            part2_grid[x][y] = 0;
        }
    }

    // Process each command and show intermediate results
    for (i = 0; i < num_commands; i++) {
        process_command_part2(part2_grid, &commands[i], &brightness_total);
        cprintf("Step %d: %lu brightness\r\n", i+1, brightness_total);
    }

    return brightness_total;
}

// Demonstration with small grid (works on real C64)
void demo_with_small_grid(void) {
    cprintf("DEMO: turn on 0,0-49,49 -> 2500 on\r\n");
    cprintf("DEMO: toggle 0,0-49,0 -> 2450 on\r\n");
    cprintf("DEMO: turn off 24,24-25,25 -> 2446 on\r\n");
}

void test_part1(void) {
    int i;
    LightCommand commands[PART1_TEST_COUNT];
    unsigned long result;

    cprintf("PART 1:\r\n");

    // Parse all instructions
    for (i = 0; i < PART1_TEST_COUNT; i++) {
        parse_instruction(part1_instructions[i], &commands[i]);
    }

    // Simulate on actual 50x50 grid
    result = simulate_part1_full(commands, PART1_TEST_COUNT);

    if (result == part1_expected[PART1_TEST_COUNT - 1]) {
        cprintf("FINAL: PASS\r\n");
    } else {
        cprintf("FINAL: FAIL (got %lu, expected %lu)\r\n", result, part1_expected[PART1_TEST_COUNT - 1]);
    }
}

void test_part2(void) {
    int i;
    LightCommand commands[PART2_TEST_COUNT];
    unsigned long result;

    cprintf("PART 2:\r\n");

    // Parse all instructions
    for (i = 0; i < PART2_TEST_COUNT; i++) {
        parse_instruction(part2_instructions[i], &commands[i]);
    }

    // Simulate on actual 50x50 brightness grid
    result = simulate_part2_full(commands, PART2_TEST_COUNT);

    if (result == part2_expected[PART2_TEST_COUNT - 1]) {
        cprintf("FINAL: PASS\r\n");
    } else {
        cprintf("FINAL: FAIL (got %lu, expected %lu)\r\n", result, part2_expected[PART2_TEST_COUNT - 1]);
    }
}

int main(void) {
    clrscr();

    cprintf("AOC 2015 Day 6 - 50x50 Grid\r\n");
    cprintf("==========================\r\n");

    // Show small grid demonstration
    demo_with_small_grid();
    cprintf("\r\n");

    // Run the actual tests
    test_part1();
    test_part2();

    cprintf("\r\nPress any key...\r\n");
    cgetc();

    return 0;
}
