#include <stdio.h>
#include <stdlib.h>
#include "algorithm.h"

int main(void) {
  printf("Testing Day 25 algorithm...\n");

  // Test key cases
  struct {
    int row, col;
    long expected;
  } tests[] = {
    {1, 1, 20151125L}, {1, 2, 18749137L}, {1, 3, 17289845L},
    {2, 1, 31916031L}, {2, 2, 21629792L}, {3, 3, 1601130L},
    {6, 6, 27995004L}
  };

  int num_tests = sizeof(tests) / sizeof(tests[0]);
  int passed = 0;
  int i;

  for (i = 0; i < num_tests; i++) {
    long result = get_code(tests[i].row, tests[i].col);
    if (result == tests[i].expected) {
      printf("PASS: (%d,%d) = %ld\n", tests[i].row, tests[i].col, result);
      passed++;
    } else {
      printf("FAIL: (%d,%d) got %ld, expected %ld\n",
             tests[i].row, tests[i].col, result, tests[i].expected);
    }
  }

  printf("\nPassed %d/%d tests\n", passed, num_tests);

  if (passed == num_tests) {
    printf("\nTesting puzzle input (2947, 3029)...\n");
    long result = get_code(2947, 3029);
    printf("Code: %ld\n", result);
  }

  return 0;
}
