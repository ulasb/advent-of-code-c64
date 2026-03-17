#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function Prototypes */
void run_tests(void);
int solve_part1(int *instructions, int count);
int solve_part2(int *instructions, int count);

int main(void) {
  bgcolor(COLOR_BLUE);
  bordercolor(COLOR_LIGHTBLUE);
  textcolor(COLOR_WHITE);
  clrscr();
  gotoxy(0, 0);

  cprintf("ADVENT OF CODE 2017 - DAY 5\r\n");
  cprintf("A MAZE OF TWISTY TRAMPOLINES\r\n");
  cprintf("============================\r\n");

  run_tests();

  cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
  cgetc();

  return 0;
}

void run_tests(void) {
  int example_template[] = {0, 3, 0, 1, -3};
  int count = sizeof(example_template) / sizeof(example_template[0]);
  int test_data[5];
  int p1_res, p2_res;

  cprintf("\r\nRUNNING TESTS...\r\n");

  cprintf("TEST PART 1: ");
  memcpy(test_data, example_template, sizeof(example_template));
  p1_res = solve_part1(test_data, count);
  if (p1_res == 5)
    cprintf("PASS\r\n");
  else
    cprintf("FAIL (got %d)\r\n", p1_res);

  cprintf("TEST PART 2: ");
  memcpy(test_data, example_template, sizeof(example_template));
  p2_res = solve_part2(test_data, count);
  if (p2_res == 10)
    cprintf("PASS\r\n");
  else
    cprintf("FAIL (got %d)\r\n", p2_res);
}

int solve_part1(int *instructions, int count) {
  int idx = 0;
  int steps = 0;
  int offset;

  while (idx >= 0 && idx < count) {
    offset = instructions[idx];
    instructions[idx]++;
    idx += offset;
    steps++;
  }

  return steps;
}

int solve_part2(int *instructions, int count) {
  int idx = 0;
  int steps = 0;
  int offset;

  while (idx >= 0 && idx < count) {
    offset = instructions[idx];
    if (offset >= 3) {
      instructions[idx]--;
    } else {
      instructions[idx]++;
    }
    idx += offset;
    steps++;
  }

  return steps;
}
