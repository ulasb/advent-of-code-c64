#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function Prototypes */
void run_tests(void);
int solve_part1(int rows[][16], int row_lengths[], int row_count);
int solve_part2(int rows[][16], int row_lengths[], int row_count);

int main(void) {
  bgcolor(COLOR_BLUE);
  bordercolor(COLOR_LIGHTBLUE);
  textcolor(COLOR_WHITE);
  clrscr();
  gotoxy(0, 0);

  cprintf("ADVENT OF CODE 2017 - DAY 2\r\n");
  cprintf("CORRUPTION CHECKSUM\r\n");
  cprintf("============================\r\n");

  run_tests();

  cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
  cgetc();

  return 0;
}

void run_tests(void) {
  /* Part 1 Example Data */
  int p1_rows[3][16] = {
    {5, 1, 9, 5},
    {7, 5, 3},
    {2, 4, 6, 8}
  };
  int p1_lengths[3] = {4, 3, 4};
  int p1_res;

  /* Part 2 Example Data */
  int p2_rows[3][16] = {
    {5, 9, 2, 8},
    {9, 4, 7, 3},
    {3, 8, 6, 5}
  };
  int p2_lengths[3] = {4, 4, 4};
  int p2_res;

  cprintf("\r\nRUNNING TESTS...\r\n");

  /* Part 1 Test */
  cprintf("TEST PART 1: ");
  p1_res = solve_part1(p1_rows, p1_lengths, 3);
  if (p1_res == 18)
    cprintf("PASS (18)\r\n");
  else
    cprintf("FAIL (got %d, expected 18)\r\n", p1_res);

  /* Part 2 Test */
  cprintf("TEST PART 2: ");
  p2_res = solve_part2(p2_rows, p2_lengths, 3);
  if (p2_res == 9)
    cprintf("PASS (9)\r\n");
  else
    cprintf("FAIL (got %d, expected 9)\r\n", p2_res);
}

int solve_part1(int rows[][16], int row_lengths[], int row_count) {
  int checksum = 0;
  int i, j;
  int min_val, max_val;
  int current;

  for (i = 0; i < row_count; i++) {
    if (row_lengths[i] == 0) continue;
    
    min_val = rows[i][0];
    max_val = rows[i][0];
    
    for (j = 1; j < row_lengths[i]; j++) {
      current = rows[i][j];
      if (current < min_val) min_val = current;
      if (current > max_val) max_val = current;
    }
    checksum += (max_val - min_val);
  }
  
  return checksum;
}

int solve_part2(int rows[][16], int row_lengths[], int row_count) {
  int checksum = 0;
  int i, j, k;
  int a, b;
  int found;

  for (i = 0; i < row_count; i++) {
    found = 0;
    for (j = 0; j < row_lengths[i]; j++) {
      for (k = 0; k < row_lengths[i]; k++) {
        if (j == k) continue;
        
        a = rows[i][j];
        b = rows[i][k];
        
        if (b != 0 && a % b == 0) {
          checksum += (a / b);
          found = 1;
          break;
        }
      }
      if (found) break;
    }
  }
  
  return checksum;
}
