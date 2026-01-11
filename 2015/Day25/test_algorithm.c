#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CODE 20151125L
#define MULTIPLIER 252533L
#define MODULUS 33554393L

long safe_mod_mult(long a, long b, long modulus) {
  long result = 0;
  a = a % modulus;
  b = b % modulus;

  while (b > 0) {
    if (b % 2 == 1) {
      result = (result + a) % modulus;
    }
    a = (a * 2) % modulus;
    b = b / 2;
  }

  return result;
}

long mod_pow(long base, long exponent, long modulus) {
  long result = 1;
  base = base % modulus;

  while (exponent > 0) {
    if (exponent % 2 == 1) {
      result = safe_mod_mult(result, base, modulus);
    }
    base = safe_mod_mult(base, base, modulus);
    exponent = exponent / 2;
  }

  return result;
}

long get_index(int row, int col) {
  long diagonal = (long)row + col - 2;
  return (diagonal * (diagonal + 1)) / 2 + col;
}

long get_code(int row, int col) {
  long index = get_index(row, col);
  long exponent = index - 1;
  long multiplier_pow = mod_pow(MULTIPLIER, exponent, MODULUS);
  return safe_mod_mult(INITIAL_CODE, multiplier_pow, MODULUS);
}

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
