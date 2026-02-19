#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global buffers for C64 stability */
char current_password[16];
char temp_buffer[16];

/* Function Prototypes */
void swap_position(char *pw, int x, int y);
void swap_letter(char *pw, char x, char y);
void rotate_left(char *pw, int steps);
void rotate_right(char *pw, int steps);
void rotate_based_on_position(char *pw, char letter);
void reverse_positions(char *pw, int x, int y);
void move_position(char *pw, int x, int y);

/* Reverse operations */
void undo_rotate_based_on_position(char *pw, char letter);

void run_tests(void);

int main(void) {
  bgcolor(COLOR_BLUE);
  bordercolor(COLOR_LIGHTBLUE);
  textcolor(COLOR_WHITE);
  clrscr();
  gotoxy(0, 0);

  cprintf("ADVENT OF CODE 2016 - DAY 21\r\n");
  cprintf("SCRAMBLED LETTERS AND HASH\r\n");
  cprintf("============================\r\n");

  run_tests();

  cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
  cgetc();

  return 0;
}

void swap_position(char *pw, int x, int y) {
  char temp = pw[x];
  pw[x] = pw[y];
  pw[y] = temp;
}

void swap_letter(char *pw, char x, char y) {
  int i;
  int len = strlen(pw);
  for (i = 0; i < len; i++) {
    if (pw[i] == x) {
      pw[i] = y;
    } else if (pw[i] == y) {
      pw[i] = x;
    }
  }
}

void rotate_left(char *pw, int steps) {
  int len = strlen(pw);
  if (len == 0) return;
  steps %= len;
  if (steps == 0) return;
  
  memcpy(temp_buffer, pw, steps);
  memmove(pw, pw + steps, len - steps);
  memcpy(pw + len - steps, temp_buffer, steps);
}

void rotate_right(char *pw, int steps) {
  int len = strlen(pw);
  if (len == 0) return;
  steps %= len;
  if (steps == 0) return;
  
  memcpy(temp_buffer, pw + len - steps, steps);
  memmove(pw + steps, pw, len - steps);
  memcpy(pw, temp_buffer, steps);
}

void rotate_based_on_position(char *pw, char letter) {
  int i, len = strlen(pw);
  int pos = -1;
  int steps;
  for (i = 0; i < len; i++) {
    if (pw[i] == letter) {
      pos = i;
      break;
    }
  }
  if (pos == -1) return;
  steps = 1 + pos + (pos >= 4 ? 1 : 0);
  rotate_right(pw, steps);
}

void undo_rotate_based_on_position(char *pw, char letter) {
  int i, len = strlen(pw);
  char candidate[16];
  char check[16];
  
  for (i = 0; i < len; i++) {
    strcpy(candidate, pw);
    rotate_left(candidate, i);
    strcpy(check, candidate);
    rotate_based_on_position(check, letter);
    if (strcmp(check, pw) == 0) {
      strcpy(pw, candidate);
      return;
    }
  }
}

void reverse_positions(char *pw, int x, int y) {
  char temp;
  while (x < y) {
    temp = pw[x];
    pw[x] = pw[y];
    pw[y] = temp;
    x++;
    y--;
  }
}

void move_position(char *pw, int x, int y) {
  char c = pw[x];
  int len = strlen(pw);
  if (x < y) {
    memmove(pw + x, pw + x + 1, y - x);
  } else {
    memmove(pw + y + 1, pw + y, x - y);
  }
  pw[y] = c;
}

void assert_str(const char *actual, const char *expected, const char *test_name) {
  if (strcmp(actual, expected) == 0) {
    cprintf("%-25s: PASS\r\n", test_name);
  } else {
    cprintf("%-25s: FAIL\r\n", test_name);
    cprintf("  EXPECTED: [%s]\r\n", expected);
    cprintf("  ACTUAL  : [%s]\r\n", actual);
  }
}

void run_tests(void) {
  cprintf("\r\nRUNNING FUNDAMENTAL TESTS...\r\n");

  /* swap position 4 with position 0 */
  strcpy(current_password, "abcde");
  swap_position(current_password, 4, 0);
  assert_str(current_password, "ebcda", "SWAP POS 4,0");

  /* swap letter d with letter b */
  strcpy(current_password, "ebcda");
  swap_letter(current_password, 'd', 'b');
  assert_str(current_password, "edcba", "SWAP LETTER D,B");

  /* rotate left 1 */
  strcpy(current_password, "edcba");
  rotate_left(current_password, 1);
  assert_str(current_password, "dcbae", "ROTATE LEFT 1");

  /* rotate right 1 */
  strcpy(current_password, "dcbae");
  rotate_right(current_password, 1);
  assert_str(current_password, "edcba", "ROTATE RIGHT 1");

  /* rotate based on position of letter b */
  strcpy(current_password, "abdec");
  rotate_based_on_position(current_password, 'b');
  assert_str(current_password, "ecabd", "ROTATE BASED B");

  /* reverse positions 0 through 4 */
  strcpy(current_password, "edcba");
  reverse_positions(current_password, 0, 4);
  assert_str(current_password, "abcde", "REVERSE 0-4");

  /* move position 1 to position 4 */
  strcpy(current_password, "bcdea");
  move_position(current_password, 1, 4);
  assert_str(current_password, "bdeac", "MOVE POS 1 TO 4");

  /* undo rotate based on position */
  strcpy(current_password, "ecabd");
  undo_rotate_based_on_position(current_password, 'b');
  assert_str(current_password, "abdec", "UNDO ROTATE BASED B");
}
