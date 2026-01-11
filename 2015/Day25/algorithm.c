/* algorithm.c - Core algorithm implementation for Day 25 */

#include "algorithm.h"

long safe_mod_mult(long a, long b, long modulus) {
  /* Safe modular multiplication to avoid overflow */
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
  /* Modular exponentiation using binary exponentiation with safe multiplication */
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
  /* Calculate the 1-based index in the diagonal sequence */
  long diagonal = (long)row + col - 2;
  return (diagonal * (diagonal + 1)) / 2 + col;
}

long get_code(int row, int col) {
  long index = get_index(row, col);
  long exponent = index - 1;

  /* Using modular exponentiation for efficiency */
  long multiplier_pow = mod_pow(MULTIPLIER, exponent, MODULUS);
  return safe_mod_mult(INITIAL_CODE, multiplier_pow, MODULUS);
}
