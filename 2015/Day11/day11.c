#include <conio.h>
#include <stdio.h>
#include <string.h>

#define PWD_LEN 8

// Global password buffer
char pwd[PWD_LEN + 1];

// Function: increment_pwd
// Increments the global password buffer.
void increment_pwd(void) {
  // static locals for speed (avoid stack)
  static unsigned char i;

  // Start from the rightmost character
  for (i = PWD_LEN - 1; i < PWD_LEN;
       i--) { // i is unsigned, so >= 0 check is tricky. i < PWD_LEN relies on
              // underflow.
    // Wait, unsigned char i. 7, 6... 0. 0-- becomes 255.
    // 255 is >= PWD_LEN (8). Loop terminates.
    if (pwd[i] < 'z') {
      pwd[i]++;
      break;
    } else {
      pwd[i] = 'a';
    }
  }
}

// Function: fast_skip
// Checks if password contains i, o, l. If so, increments and resets.
// Returns 1 if skip happened, 0 otherwise.
unsigned char fast_skip(void) {
  static unsigned char i, j;
  for (i = 0; i < PWD_LEN; i++) {
    if (pwd[i] == 'i' || pwd[i] == 'o' || pwd[i] == 'l') {
      pwd[i]++; // Skip the forbidden char to the next one
      // Reset all rest to 'a' to get the lowest possible valid suffix
      for (j = i + 1; j < PWD_LEN; j++) {
        pwd[j] = 'a';
      }
      return 1;
    }
  }
  return 0;
}

// Function: is_valid_pwd
// Checks the 3 rules on global pwd.
unsigned char is_valid_pwd(void) {
  static unsigned char i;
  static unsigned char has_straight;
  static unsigned char pair_count;
  static char first_pair_char;

  has_straight = 0;
  pair_count = 0;
  first_pair_char = 0;

  // Rule 2: No i, o, l
  // Handled by valid_skip optimization in main loop.

  // Rule 1: Increasing straight of 3 (abc)
  for (i = 0; i < PWD_LEN - 2; i++) {
    // Check pwd[i], pwd[i+1], pwd[i+2]
    if (pwd[i + 1] == pwd[i] + 1 && pwd[i + 2] == pwd[i] + 2) {
      has_straight = 1;
      break;
    }
  }
  if (!has_straight)
    return 0;

  // Rule 3: Two different non-overlapping pairs
  for (i = 0; i < PWD_LEN - 1; i++) {
    if (pwd[i] == pwd[i + 1]) {
      if (pair_count == 0) {
        pair_count++;
        first_pair_char = pwd[i];
        i++; // Skip next to ensure non-overlapping
      } else {
        // Check if it's a different pair (e.g. 'aa' then 'bb')
        if (pwd[i] != first_pair_char) {
          pair_count++;
          break;
        }
      }
    }
  }

  if (pair_count < 2)
    return 0;

  return 1;
}

void solve(const char *label) {
  static unsigned long attempts;
  attempts = 0;

  // Ensure we start clean
  fast_skip();

  cprintf("%s: Searching...\r\n", label);

  // Loop until valid
  while (1) {
    attempts++;

    // Progress display every 2048 iterations
    if ((attempts & 2047) == 0) {
      cprintf("\r%s (%lu)", pwd, attempts);
    }

    increment_pwd();

    // Optimization: Check forbidden chars causing big skips
    // We loop fast_skip in case multiple skips are needed (unlikely but safe),
    // and crucially, we FALL THROUGH to validation. The result of a skip
    // is a valid candidate that must be checked, not skipped.
    while (fast_skip())
      ;

    // Check remaining rules (Straight, Pairs)
    if (is_valid_pwd()) {
      break;
    }
  }
  cprintf("\rFOUND: %s (after %lu)   \r\n", pwd, attempts);
}

int main() {
  // Set up screen
  bgcolor(COLOR_BLACK);
  bordercolor(COLOR_BLACK);
  textcolor(COLOR_GRAY3); // readable on black
  clrscr();

  // Initial password
  strcpy(pwd, "cqjxjnds");

  cprintf("Advent of Code 2015 - Day 11\r\n");
  cprintf("Input: %s\r\n\r\n", pwd);

  solve("Part 1"); // Find next

  // For Part 2, find next after Part 1
  solve("Part 2");

  // Wait for key
  cprintf("\r\nDone. Press any key.\r\n");
  while (!kbhit())
    ;
  cgetc();

  return 0;
}
