#include <conio.h>
#include <stdio.h>
#include <string.h>

// Constants
#define UINT16_MASK 0xFFFF  // 16-bit unsigned integer mask
#define MAX_WIRES 16        // Maximum number of wires for test circuit
#define MAX_EXPR_LEN 32     // Maximum expression length

// Temporary buffer for safe tokenization (avoids modifying original expressions)
char temp_expr[MAX_EXPR_LEN];

// Wire storage structure
typedef struct {
    char name[4];           // Wire name (like "a", "x", "y")
    char expression[MAX_EXPR_LEN]; // Expression (like "123", "x AND y", "NOT x")
    int resolved;           // Whether this wire has been resolved
    unsigned int value;     // Cached resolved value
} Wire;

// Global wire array for C64 compatibility
Wire wires[MAX_WIRES];
int wire_count = 0;

// Test circuit from problem description
#define TEST_WIRE_COUNT 8
const char* test_circuit[TEST_WIRE_COUNT] = {
    "123 -> x",
    "456 -> y",
    "x AND y -> d",
    "x OR y -> e",
    "x LSHIFT 2 -> f",
    "y RSHIFT 2 -> g",
    "NOT x -> h",
    "NOT y -> i"
};

const unsigned int expected_values[8] = {
    123U,   // x
    456U,   // y
    72U,    // d: x AND y
    507U,   // e: x OR y
    492U,   // f: x LSHIFT 2
    114U,   // g: y RSHIFT 2
    65412U, // h: NOT x
    65079U  // i: NOT y
};

// Simple atoi for C64 (no stdlib dependency)
unsigned int simple_atoi(const char* str) {
    unsigned int result = 0;
    int i = 0;
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return result;
}

// Check if string is a number
int is_number(const char* str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] < '0' || str[i] > '9') {
            return 0;
        }
        i++;
    }
    return i > 0;
}

// Find wire by name, return index or -1 if not found
int find_wire(const char* name) {
    int i;
    for (i = 0; i < wire_count; i++) {
        if (strcmp(wires[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Recursively resolve the value of a wire/expression
unsigned int resolve_wire(int wire_index) {
    Wire* wire = &wires[wire_index];
    char* parts[4]; // Max 4 parts for expressions like "x LSHIFT 2"
    int part_count = 0;
    char* token;
    char* next_token;
    unsigned int result;

    // Check if already resolved
    if (wire->resolved) {
        return wire->value;
    }

    // Parse the expression (copy to temp buffer to avoid modifying original)
    strcpy(temp_expr, wire->expression);
    part_count = 0;
    token = temp_expr;
    next_token = NULL;

    // Simple tokenization (space-separated) - safe on temp buffer
    while (*token != '\0' && part_count < 4) {
        // Skip leading spaces
        while (*token == ' ') token++;

        if (*token == '\0') break;

        next_token = token;
        while (*next_token != '\0' && *next_token != ' ') next_token++;

        if (*next_token != '\0') {
            *next_token = '\0';
            next_token++;
        }

        parts[part_count++] = token;
        token = next_token;
    }

    if (part_count == 1) {
        // Direct number or wire reference
        if (is_number(parts[0])) {
            result = simple_atoi(parts[0]);
        } else {
            // Wire reference - resolve recursively
            int ref_index = find_wire(parts[0]);
            if (ref_index == -1) {
                // Error: wire not found - report and return 0
                cprintf("ERROR: Wire '%s' not found\r\n", parts[0]);
                result = 0;
            } else {
                result = resolve_wire(ref_index);
            }
        }
    } else if (part_count == 2) {
        // NOT operation
        if (strcmp(parts[0], "NOT") == 0) {
            int ref_index = find_wire(parts[1]);
            if (ref_index == -1) {
                cprintf("ERROR: Wire '%s' not found\r\n", parts[1]);
                result = 0;
            } else {
                unsigned int operand = resolve_wire(ref_index);
                result = (~operand) & UINT16_MASK;
            }
        } else {
            // Error: invalid unary operation
            result = 0;
        }
    } else if (part_count == 3) {
        // Binary operations
        int left_index = find_wire(parts[0]);
        int right_index = -1;
        unsigned int left_val, right_val;

        if (left_index == -1) {
            // Left operand is a number
            left_val = simple_atoi(parts[0]);
        } else {
            left_val = resolve_wire(left_index);
        }

        if (strcmp(parts[1], "AND") == 0 || strcmp(parts[1], "OR") == 0) {
            // Right operand could be wire or number
            if (is_number(parts[2])) {
                right_val = simple_atoi(parts[2]);
            } else {
                right_index = find_wire(parts[2]);
                if (right_index == -1) {
                    cprintf("ERROR: Wire '%s' not found\r\n", parts[2]);
                    right_val = 0;
                } else {
                    right_val = resolve_wire(right_index);
                }
            }

            if (strcmp(parts[1], "AND") == 0) {
                result = (left_val & right_val) & UINT16_MASK;
            } else { // OR
                result = (left_val | right_val) & UINT16_MASK;
            }
        } else if (strcmp(parts[1], "LSHIFT") == 0) {
            // Shift amount is always a number
            unsigned int shift_amount = simple_atoi(parts[2]);
            result = (left_val << shift_amount) & UINT16_MASK;
        } else if (strcmp(parts[1], "RSHIFT") == 0) {
            // Shift amount is always a number
            unsigned int shift_amount = simple_atoi(parts[2]);
            result = (left_val >> shift_amount) & UINT16_MASK;
        } else {
            // Error: unknown operation
            result = 0;
        }
    } else {
        // Error: invalid expression format
        result = 0;
    }

    // Cache the result
    wire->resolved = 1;
    wire->value = result;
    return result;
}

// Parse a single wire definition line
void parse_wire_definition(const char* line) {
    char* arrow_pos;
    int expr_len;
    int name_len;

    if (wire_count >= MAX_WIRES) {
        return; // Too many wires
    }

    // Find the " -> " separator (work on original string)
    arrow_pos = strstr(line, " -> ");
    if (arrow_pos == NULL) {
        return; // Invalid format
    }

    // Calculate lengths for copying
    expr_len = arrow_pos - line;
    name_len = strlen(arrow_pos + 4);

    // Check bounds
    if (expr_len >= MAX_EXPR_LEN || name_len >= 4) {
        return; // Too long
    }

    // Copy expression and name
    {
        char* expression = wires[wire_count].expression;
        char* wire_name = wires[wire_count].name;
        int i;

        // Copy expression part
        for (i = 0; i < expr_len; i++) {
            expression[i] = line[i];
        }
        expression[i] = '\0';

        // Copy wire name part
        {
            const char* name_src = arrow_pos + 4;
            for (i = 0; i < name_len; i++) {
                wire_name[i] = name_src[i];
            }
            wire_name[i] = '\0';
        }
    }

    // Initialize the wire
    wires[wire_count].resolved = 0;
    wires[wire_count].value = 0;
    wire_count++;
}

// Initialize the test circuit
void initialize_test_circuit(void) {
    int i;
    wire_count = 0;

    for (i = 0; i < TEST_WIRE_COUNT; i++) {
        parse_wire_definition(test_circuit[i]);
    }
}

// Run the tests and validate results
void run_tests(void) {
    int i;
    int pass_count = 0;
    int wire_index;
    unsigned int actual_value;
    unsigned int expected_value;
    const char* wire_names[8] = {"x", "y", "d", "e", "f", "g", "h", "i"};

    for (i = 0; i < TEST_WIRE_COUNT; i++) {
        wire_index = find_wire(wire_names[i]);
        if (wire_index == -1) {
            cprintf("%s: ERROR\r\n", wire_names[i]);
            continue;
        }

        actual_value = resolve_wire(wire_index);
        expected_value = expected_values[i];

        if (actual_value == expected_value) {
            pass_count++;
        } else {
            cprintf("%s FAIL: %u != %u\r\n", wire_names[i], actual_value, expected_value);
        }
    }

    cprintf("RESULT: %d/%d PASSED\r\n", pass_count, TEST_WIRE_COUNT);

    if (pass_count == TEST_WIRE_COUNT) {
        cprintf("SUCCESS!\r\n");
    } else {
        cprintf("FAILED!\r\n");
    }
}

// Display the circuit for verification
void display_circuit(void) {
    int i;
    cprintf("CIRCUIT DEFINITIONS:\r\n");
    cprintf("====================\r\n");

    for (i = 0; i < wire_count; i++) {
        cprintf("%s <- %s\r\n", wires[i].name, wires[i].expression);
    }
    cprintf("\r\n");
}

int main(void) {
    clrscr();

    cprintf("AOC 2015 DAY 7\r\n");
    cprintf("==============\r\n\r\n");

    // Initialize the test circuit
    initialize_test_circuit();

    // Run the tests
    run_tests();

    cprintf("\r\nPress key...\r\n");
    cgetc();

    return 0;
}
