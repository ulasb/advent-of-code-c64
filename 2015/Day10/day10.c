/*
 * Advent of Code 2015 - Day 10
 * Elves Look, Elves Say
 *
 * C64 implementation using cc65
 */

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define MAX_SEQUENCE_LEN 4096 // Safe buffer size for C64 RAM (2 buffers = 8KB)

// Buffers for sequence generation
// We use two buffers and swap them to avoid in-place modification complexity
// making them static to avoid stack overflow
char buffer_a[MAX_SEQUENCE_LEN];
char buffer_b[MAX_SEQUENCE_LEN];

// Test cases from problem description
typedef struct {
  const char *input;
  const char *expected;
  const char *description;
} TestCase;

#define TEST_CASE_COUNT 5
const TestCase test_cases[TEST_CASE_COUNT] = {
    {"1", "11", "1 becomes 11"},
    {"11", "21", "11 becomes 21"},
    {"21", "1211", "21 becomes 1211"},
    {"1211", "111221", "1211 becomes 111221"},
    {"111221", "312211", "111221 becomes 312211"}};

// Input sequence for the puzzle
const char *PUZZLE_INPUT = "1113122113";

// Helper to convert integer to string and append to buffer
// Returns new position in buffer
char *append_int(char *buffer, int value) {
  char temp[8];
  int i = 0;
  int j = 0;

  // Handle 0 explicitly (though count >= 1 always)
  if (value == 0) {
    *buffer++ = '0';
    return buffer;
  }

  // Extract digits in reverse
  while (value > 0) {
    temp[i++] = (value % 10) + '0';
    value /= 10;
  }

  // Copy in correct order
  for (j = i - 1; j >= 0; j--) {
    *buffer++ = temp[j];
  }

  return buffer;
}

// Generate the next sequence
// Returns length of new sequence, or -1 if overflow
int generate_next_sequence(const char *input, char *output, int max_len) {
  const char *current = input;
  char *dest = output;
  char current_char;
  int count;

  // We can't check remaining space precisely for every char efficiently,
  // but append_int and writes should be safe if we are conservative.
  // For this simple implementation, we check basically.

  while (*current != '\0') {
    current_char = *current;
    count = 1;

    // Count consecutive identical digits
    while (current[1] == current_char) {
      count++;
      current++;
    }

    // Check for buffer overflow risk.
    // For this problem, count is always a single digit. We need space for:
    // - 1 byte for the count
    // - 1 byte for the digit
    // - 1 byte for the null terminator at the end.
    // So we check if we have at least 3 bytes of space left in the buffer.
    if ((dest - output) >= (max_len - 3)) {
      return -1; // Overflow
    }

    // Write count
    dest = append_int(dest, count);

    // Write digit
    *dest++ = current_char;

    // Move to next group
    current++;
  }

  *dest = '\0';
  return (int)(dest - output);
}

// Run unit tests
void run_tests(void) {
  int i;
  int pass_count = 0;
  int result_len;

  cprintf("Running Unit Tests...\r\n");
  cprintf("---------------------\r\n");

  for (i = 0; i < TEST_CASE_COUNT; i++) {
    // Use buffer_a for output, assuming input fits in buffer (test inputs are
    // small)
    result_len =
        generate_next_sequence(test_cases[i].input, buffer_a, MAX_SEQUENCE_LEN);

    if (result_len == -1) {
      cprintf("Test %d: ERROR (Overflow)\r\n", i + 1);
      continue;
    }

    if (strcmp(buffer_a, test_cases[i].expected) == 0) {
      cprintf("Test %d: PASS\r\n", i + 1);
      pass_count++;
    } else {
      cprintf("Test %d: FAIL\r\n", i + 1);
      // Don't print strings if they are too long, but for tests they are short
      cprintf("  Exp: %s\r\n", test_cases[i].expected);
      cprintf("  Got: %s\r\n", buffer_a);
    }
  }

  cprintf("\r\nResult: %d/%d Passed\r\n", pass_count, TEST_CASE_COUNT);

  if (pass_count == TEST_CASE_COUNT) {
    cprintf("SUCCESS!\r\n");
  } else {
    cprintf("FAILED!\r\n");
  }
}

// Demonstrate the growth
void demonstrate_growth(void) {
  int i;
  int len;
  char *src = buffer_a;
  char *dst = buffer_b;
  char *temp;

  // Clear screen before starting the new phase to avoid wrapping/overwriting
  clrscr();
  cprintf("Demonstrating Growth:\r\n");
  cprintf("-------------------\r\n");

  // Start with puzzle input
  snprintf(src, MAX_SEQUENCE_LEN, "%s", PUZZLE_INPUT);
  cprintf("Start: %s\r\n", src);
  cprintf("Length: %d\r\n\r\n", strlen(src));

  // Run for a few steps until overflow
  for (i = 1; i <= 20; i++) {
    len = generate_next_sequence(src, dst, MAX_SEQUENCE_LEN);

    if (len == -1) {
      // Clear screen to show the error clearly without wrapping garbage
      clrscr();
      textcolor(COLOR_RED);
      cprintf("STOPPED: Memory Limit Reached\r\n");
      cprintf("=============================\r\n\r\n");

      cprintf("Step %d would exceed buffer size.\r\n", i);
      cprintf("Max buffer: %d bytes\r\n", MAX_SEQUENCE_LEN);
      cprintf("Previous length: %d\r\n", strlen(src));

      // Wait for user to acknowledge the cleanup
      cprintf("\r\nPress key to continue...\r\n");
      cgetc();
      textcolor(COLOR_GREEN); // Restore color
      return;
    }

    cprintf("Step %d: Len %d\r\n", i, len);

    // Swap buffers
    temp = src;
    src = dst;
    dst = temp;

    // Key check to abort
    if (kbhit()) {
      cgetc(); // consume key
      cprintf("\r\nAborted by user.\r\n");
      return;
    }
  }
}

int main(void) {
  // Clear screen
  clrscr();
  bgcolor(COLOR_BLACK);
  textcolor(COLOR_GREEN);
  bordercolor(COLOR_BLACK);

  cprintf("AoC 2015 Day 10 - C64\r\n");
  cprintf("=====================\r\n\r\n");

  run_tests();

  cprintf("\r\nPress any key to start extraction...\r\n");
  cgetc();

  // Run demonstration
  demonstrate_growth();

  // Final status screen
  clrscr();
  cprintf("PROGRAM FINISHED\r\n");
  cprintf("----------------\r\n");
  cprintf("The sequence grows too fast for\r\n");
  cprintf("C64 memory (40-50 steps needed).\r\n");
  cprintf("\r\nAlgorithm is correct based on tests.\r\n");

  cprintf("\r\nPress key to exit back to BASIC.\r\n");
  cgetc();

  // Clear screen on exit to be clean
  clrscr();
  return 0;
}
