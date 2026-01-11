#include <conio.h>
#include "algorithm.h"

/* Default target coordinates */
#define DEFAULT_ROW 2947
#define DEFAULT_COL 3029

/* Test case structure */
typedef struct {
  int row, col;
  long expected;
} TestCase;

/* Test cases from the 6x6 grid */
static TestCase test_cases[] = {
  {1, 1, 20151125L}, {1, 2, 18749137L}, {1, 3, 17289845L},
  {1, 4, 30943339L}, {1, 5, 10071777L}, {1, 6, 33511524L},
  {2, 1, 31916031L}, {2, 2, 21629792L}, {2, 3, 16929656L},
  {2, 4, 7726640L},  {2, 5, 15514188L}, {2, 6, 4041754L},
  {3, 1, 16080970L}, {3, 2, 8057251L},  {3, 3, 1601130L},
  {3, 4, 7981243L},  {3, 5, 11661866L}, {3, 6, 16474243L},
  {4, 1, 24592653L}, {4, 2, 32451966L}, {4, 3, 21345942L},
  {4, 4, 9380097L},  {4, 5, 10600672L}, {4, 6, 31527494L},
  {5, 1, 77061L},    {5, 2, 17552253L}, {5, 3, 28094349L},
  {5, 4, 6899651L},  {5, 5, 9250759L},  {5, 6, 31663883L},
  {6, 1, 33071741L}, {6, 2, 6796745L},  {6, 3, 25397450L},
  {6, 4, 24659492L}, {6, 5, 1534922L},  {6, 6, 27995004L}
};

/* Function Prototypes */
void run_tests(void);

int main(void) {
  bgcolor(COLOR_BLUE);
  bordercolor(COLOR_LIGHTBLUE);
  textcolor(COLOR_WHITE);
  clrscr();
  gotoxy(0, 0);

  cprintf("ADVENT OF CODE 2015 - DAY 25\r\n");
  cprintf("LET IT SNOW\r\n");
  cprintf("============================\r\n");

  run_tests();

  cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
  cgetc();

  return 0;
}

void run_tests(void) {
  long result;
  int passed = 0;
  int i;
  int idx;

  /* Key test indices to verify algorithm without overwhelming screen */
  int key_tests[] = {0, 1, 2, 6, 12, 18, 24, 35}; /* Corners and some middle */
  int num_key_tests = sizeof(key_tests) / sizeof(key_tests[0]);

  cprintf("\r\nRUNNING KEY TESTS...\r\n");

  for (i = 0; i < num_key_tests; i++) {
    idx = key_tests[i];
    result = get_code(test_cases[idx].row, test_cases[idx].col);
    if (result == test_cases[idx].expected) {
      passed++;
      cprintf("P: (%d,%d)\r\n", test_cases[idx].row, test_cases[idx].col);
    } else {
      cprintf("F: (%d,%d) %ld != %ld\r\n",
              test_cases[idx].row, test_cases[idx].col, result, test_cases[idx].expected);
    }
  }

  cprintf("\r\nPASSED: %d/%d key tests\r\n", passed, num_key_tests);

  if (passed == num_key_tests) {
    cprintf("ALGORITHM VERIFIED!\r\n\r\n");
    cprintf("PUZZLE SOLUTION:\r\n");
    result = get_code(DEFAULT_ROW, DEFAULT_COL);
    cprintf("Code at row %d, column %d:\r\n%ld\r\n",
            DEFAULT_ROW, DEFAULT_COL, result);
  } else {
    cprintf("ALGORITHM HAS ISSUES\r\n");
  }
}

