#include <conio.h>
#include <string.h>


/* Function Prototypes */
void run_tests(void);
unsigned int solve_part1(const char *input);
unsigned int solve_part2(const char *input);

int main(void) {
  /* Set up the C64 screen nicely */
  bgcolor(COLOR_BLACK);
  bordercolor(COLOR_GRAY3);
  textcolor(COLOR_LIGHTBLUE);
  clrscr();
  gotoxy(0, 0);

  cprintf("ADVENT OF CODE 2017 - DAY 1\r\n");
  cprintf("INVERSE CAPTCHA\r\n");
  cprintf("============================\r\n\r\n");

  run_tests();

  cprintf("\r\nPRESS ANY KEY TO EXIT.\r\n");
  cgetc();

  return 0;
}

void print_test_result(const char *name, unsigned int result, unsigned int expected) {
  cprintf("%s: ", name);
  if (result == expected) {
    textcolor(COLOR_GREEN);
    cprintf("PASS");
  } else {
    textcolor(COLOR_RED);
    cprintf("FAIL (exp %u, got %u)", expected, result);
  }
  textcolor(COLOR_LIGHTBLUE);
  cprintf("\r\n");
}

void run_tests(void) {
  cprintf("RUNNING PART 1 TESTS...\r\n");
  print_test_result("1122", solve_part1("1122"), 3);
  print_test_result("1111", solve_part1("1111"), 4);
  print_test_result("1234", solve_part1("1234"), 0);
  print_test_result("91212129", solve_part1("91212129"), 9);

  cprintf("\r\nRUNNING PART 2 TESTS...\r\n");
  print_test_result("1212", solve_part2("1212"), 6);
  print_test_result("1221", solve_part2("1221"), 0);
  print_test_result("123425", solve_part2("123425"), 4);
  print_test_result("123123", solve_part2("123123"), 12);
  print_test_result("12131415", solve_part2("12131415"), 4);
}

unsigned int solve_part1(const char *input) {
  unsigned int total_sum = 0;
  int n = strlen(input);
  int i;

  if (n == 0) return 0;

  for (i = 0; i < n; i++) {
    int next_idx = (i + 1) % n;
    if (input[i] == input[next_idx]) {
      /* Convert char digit to integer value */
      total_sum += (input[i] - '0');
    }
  }
  return total_sum;
}

unsigned int solve_part2(const char *input) {
  unsigned int total_sum = 0;
  int n = strlen(input);
  int offset;
  int i;

  if (n == 0) return 0;
  if (n % 2 != 0) return 0; /* Problem statement says halfway around, implies even length */

  offset = n / 2;
  for (i = 0; i < n; i++) {
    int next_idx = (i + offset) % n;
    if (input[i] == input[next_idx]) {
      total_sum += (input[i] - '0');
    }
  }
  return total_sum;
}
