/* algorithm.h - Shared algorithm definitions for Day 25 */

#ifndef ALGORITHM_H
#define ALGORITHM_H

/* Constants for the weather machine sequence */
#define INITIAL_CODE 20151125L
#define MULTIPLIER 252533L
#define MODULUS 33554393L

/* Function declarations */
long safe_mod_mult(long a, long b, long modulus);
long mod_pow(long base, long exponent, long modulus);
long get_index(int row, int col);
long get_code(int row, int col);

#endif /* ALGORITHM_H */
