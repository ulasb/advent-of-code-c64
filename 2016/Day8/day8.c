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
  memset(grid, '.', sizeof(grid));
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

  memcpy(temp, grid[row], GRID_WIDTH);
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
  const char rect_prefix[] = "rect ";
  const char row_prefix[] = "rotate row y=";
  const char col_prefix[] = "rotate column x=";
  const char by_msg[] = " by ";

  if (strncmp(cmd, rect_prefix, sizeof(rect_prefix) - 1) == 0) {
    a = atoi(cmd + sizeof(rect_prefix) - 1);
    p = strchr(cmd + sizeof(rect_prefix) - 1, 'x');
    if (p) {
      b = atoi(p + 1);
      rect(a, b);
    }
  } else if (strncmp(cmd, row_prefix, sizeof(row_prefix) - 1) == 0) {
    a = atoi(cmd + sizeof(row_prefix) - 1);
    p = strstr(cmd + sizeof(row_prefix) - 1, by_msg);
    if (p) {
      b = atoi(p + sizeof(by_msg) - 1);
      rotate_row(a, b);
    }
  } else if (strncmp(cmd, col_prefix, sizeof(col_prefix) - 1) == 0) {
    a = atoi(cmd + sizeof(col_prefix) - 1);
    p = strstr(cmd + sizeof(col_prefix) - 1, by_msg);
    if (p) {
      b = atoi(p + sizeof(by_msg) - 1);
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
  cprintf("\r\nLEFT HALF (Cols 0-%d):\r\n", (GRID_WIDTH / 2) - 1);
  for (y = 0; y < GRID_HEIGHT; y++) {
    for (x = 0; x < GRID_WIDTH / 2; x++) {
      cputc(grid[y][x] == '#' ? '#' : '.');
    }
    cprintf("\r\n");
  }
  cprintf("\r\nRIGHT HALF (Cols %d-%d):\r\n", GRID_WIDTH / 2, GRID_WIDTH - 1);
  for (y = 0; y < GRID_HEIGHT; y++) {
    for (x = GRID_WIDTH / 2; x < GRID_WIDTH; x++) {
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
  int num_cmds = sizeof(test_cmds) / sizeof(test_cmds[0]);

  cprintf("\r\nRUNNING TESTS...\r\n");

  for (i = 0; i < num_cmds; i++) {
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
