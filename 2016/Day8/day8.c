#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRID_WIDTH 50
#define GRID_HEIGHT 6

/* Global grid to avoid stack issues */
char grid[GRID_HEIGHT][GRID_WIDTH];

/* Function Prototypes */
void init_grid(void);
void rect(int a, int b);
void rotate_row(int row, int by);
void rotate_col(int col, int by);
int count_pixels(void);
void display_grid(void);
void run_tests(void);

void process_command(const char *cmd);

int main(void) {
  bgcolor(COLOR_BLACK);
  bordercolor(COLOR_BLACK);
  textcolor(COLOR_WHITE);
  clrscr();
  gotoxy(0, 0);

  cprintf("ADVENT OF CODE 2016 - DAY 8\r\n");
  cprintf("TWO-FACTOR AUTHENTICATION\r\n");
  cprintf("============================\r\n");

  init_grid();
  run_tests();

  cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
  cgetc();

  return 0;
}

void init_grid(void) {
  int y, x;
  for (y = 0; y < GRID_HEIGHT; y++) {
    for (x = 0; x < GRID_WIDTH; x++) {
      grid[y][x] = '.';
    }
  }
}

void rect(int a, int b) {
  int y, x;
  for (y = 0; y < b && y < GRID_HEIGHT; y++) {
    for (x = 0; x < a && x < GRID_WIDTH; x++) {
      grid[y][x] = '#';
    }
  }
}

void rotate_row(int row, int by) {
  char temp[GRID_WIDTH];
  int x;
  if (row < 0 || row >= GRID_HEIGHT) return;
  by %= GRID_WIDTH;
  if (by == 0) return;

  for (x = 0; x < GRID_WIDTH; x++) {
    temp[x] = grid[row][x];
  }
  for (x = 0; x < GRID_WIDTH; x++) {
    grid[row][(x + by) % GRID_WIDTH] = temp[x];
  }
}

void rotate_col(int col, int by) {
  char temp[GRID_HEIGHT];
  int y;
  if (col < 0 || col >= GRID_WIDTH) return;
  by %= GRID_HEIGHT;
  if (by == 0) return;

  for (y = 0; y < GRID_HEIGHT; y++) {
    temp[y] = grid[y][col];
  }
  for (y = 0; y < GRID_HEIGHT; y++) {
    grid[(y + by) % GRID_HEIGHT][col] = temp[y];
  }
}

void process_command(const char *cmd) {
  int a, b;
  char *p;
  if (strncmp(cmd, "rect ", 5) == 0) {
    a = atoi(cmd + 5);
    p = strchr(cmd + 5, 'x');
    if (p) {
      b = atoi(p + 1);
      rect(a, b);
    }
  } else if (strncmp(cmd, "rotate row y=", 13) == 0) {
    a = atoi(cmd + 13);
    p = strstr(cmd + 13, " by ");
    if (p) {
      b = atoi(p + 4);
      rotate_row(a, b);
    }
  } else if (strncmp(cmd, "rotate column x=", 16) == 0) {
    a = atoi(cmd + 16);
    p = strstr(cmd + 16, " by ");
    if (p) {
      b = atoi(p + 4);
      rotate_col(a, b);
    }
  }
}

int count_pixels(void) {
  int y, x, count = 0;
  for (y = 0; y < GRID_HEIGHT; y++) {
    for (x = 0; x < GRID_WIDTH; x++) {
      if (grid[y][x] == '#') count++;
    }
  }
  return count;
}

void display_grid(void) {
  int y, x;
  cprintf("\r\nLEFT HALF (Cols 0-24):\r\n");
  for (y = 0; y < GRID_HEIGHT; y++) {
    for (x = 0; x < 25; x++) {
      cputc(grid[y][x] == '#' ? '#' : '.');
    }
    cprintf("\r\n");
  }
  cprintf("\r\nRIGHT HALF (Cols 25-49):\r\n");
  for (y = 0; y < GRID_HEIGHT; y++) {
    for (x = 25; x < 50; x++) {
      cputc(grid[y][x] == '#' ? '#' : '.');
    }
    cprintf("\r\n");
  }
}

void run_tests(void) {
  int i;
  int res;
  const char *test_cmds[] = {
    "rect 3x2",
    "rotate column x=1 by 1",
    "rotate row y=0 by 4",
    "rotate column x=1 by 1",
    "rect 5x1",
    "rotate row y=1 by 10",
    "rotate column x=0 by 2"
  };

  cprintf("\r\nRUNNING TESTS...\r\n");

  for (i = 0; i < 8; i++) {
    process_command(test_cmds[i]);
  }

  res = count_pixels();
  cprintf("TEST PIXEL COUNT: ");
  if (res == 10) {
    cprintf("PASS (10)\r\n");
  } else {
    cprintf("FAIL (got %d, expected 10)\r\n", res);
  }

  display_grid();
}
