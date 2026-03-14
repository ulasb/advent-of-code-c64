#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BANKS 16
#define MAX_STATES 500

/* Global buffers */
unsigned char states[MAX_STATES][MAX_BANKS];

/* Function Prototypes */
void run_tests(void);
int solve(unsigned char *initial_banks, int num_banks, int *loop_size);

int main(void) {
  bgcolor(COLOR_BLUE);
  bordercolor(COLOR_LIGHTBLUE);
  textcolor(COLOR_WHITE);
  clrscr();
  gotoxy(0, 0);

  cprintf("ADVENT OF CODE 2017 - DAY 6\r\n");
  cprintf("MEMORY REALLOCATION\r\n");
  cprintf("=========================\r\n");

  run_tests();

  cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
  cgetc();

  return 0;
}

int solve(unsigned char *initial_banks, int num_banks, int *loop_size) {
  unsigned char current_banks[MAX_BANKS];
  int cycles = 0;
  int i;
  int found = -1;
  int max_val, max_idx;
  int blocks;
  int idx;

  memcpy(current_banks, initial_banks, num_banks);

  while (cycles < MAX_STATES) {
    /* Store current state */
    memcpy(states[cycles], current_banks, num_banks);

    /* Find max */
    max_val = -1;
    max_idx = -1;
    for (i = 0; i < num_banks; i++) {
      if (current_banks[i] > max_val) {
        max_val = current_banks[i];
        max_idx = i;
      }
    }

    /* Redistribute */
    blocks = current_banks[max_idx];
    current_banks[max_idx] = 0;
    idx = (max_idx + 1) % num_banks;

    while (blocks > 0) {
      current_banks[idx]++;
      blocks--;
      idx = (idx + 1) % num_banks;
    }

    cycles++;

    /* Check if seen */
    for (i = 0; i < cycles; i++) {
      if (memcmp(states[i], current_banks, num_banks) == 0) {
        found = i;
        break;
      }
    }

    if (found != -1) {
      *loop_size = cycles - found;
      return cycles;
    }
  }

  *loop_size = 0;
  return -1; /* Out of memory/states */
}

void run_tests(void) {
  unsigned char test_banks[] = {0, 2, 7, 0};
  int loop_size = 0;
  int cycles;

  cprintf("\r\nRUNNING TESTS...\r\n");
  cprintf("TEST CASE: 0 2 7 0\r\n");

  cycles = solve(test_banks, sizeof(test_banks), &loop_size);

  cprintf("PART 1 (CYCLES): ");
  if (cycles == 5) {
    cprintf("PASS (5)\r\n");
  } else {
    cprintf("FAIL (GOT %d)\r\n", cycles);
  }

  cprintf("PART 2 (LOOP SIZE): ");
  if (loop_size == 4) {
    cprintf("PASS (4)\r\n");
  } else {
    cprintf("FAIL (GOT %d)\r\n", loop_size);
  }
}
