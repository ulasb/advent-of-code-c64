# Advent of Code 2015 - Day 13 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2015 Day 13, written in C and compiled with cc65. You can get more information, a complete problem definition, and a larger input file on [Advent of Code](http://www.adventofcode.com).

## The Problem

This year, Santa needs help with the seating arrangement for the holiday dinner. There are several guests, and each guest has a preference for sitting next to others, expressed in "happiness units."

### Part 1
The goal is to find the optimal circular seating arrangement that maximizes the total change in happiness. For each pair of neighbors (A, B), the total change includes both A's happiness sitting next to B and B's happiness sitting next to A.

### Part 2
In Part 2, you add yourself to the seating chart. Since you are neutral, the happiness change for you sitting next to anyone is 0, and the happiness change for anyone sitting next to you is also 0.

## Running the Program

### Requirements
- cc65 compiler suite
- Commodore 64 emulator (like VICE) or real C64 hardware

### Compilation
```bash
make
```

This creates `day13.prg` which can be loaded and run on a Commodore 64.

### Testing
The program includes the test case specified in the problem description for validation. The solver uses a permutation-based approach to find the optimal arrangement.

All tests should pass with "(PASS)" indicators.

## Test Cases Included

The program validates against the following test data:

- Alice, Bob, Carol, David with their respective happiness preferences.
- Expected optimal happiness for Part 1: **330**.

## Technical Notes

### Memory Usage
- **Happiness Matrix**: 10x10 array of integers (~200 bytes)
- **Person Names**: 10x16 character array (~160 bytes)
- **Permutation Stack**: Recursive implementation, safe for small sets (up to 10-12 people)

### Data Types
- **Happiness values**: `int` (16-bit signed)
- **Total Happiness**: `long` (32-bit signed) to avoid overflow during summation
- **Names**: Fixed-size character arrays

### Algorithm Implementation

The program implements a permutation solver:

1. **Matrix Storage**: Happiness values are stored in a 2D adjacency matrix.
2. **Circular Symmetry**: To avoid redundant calculations (rotations), the first person is fixed at the first position, while others are permuted.
3. **Recursive Permutation**: Uses a standard recursive swap-based permutation algorithm.
4. **Circularity**: The happiness calculation correctly wraps around the end of the array to the beginning to simulate a circular table.

### Performance Optimizations

The program implements several C64-specific optimizations:

- **Symmetry Reduction**: Fixing one person reduces permutations from $n!$ to $(n-1)!$.
- **Fast Lookup**: Names are mapped to small integer IDs for array indexing.
- **Minimal Buffers**: Avoids heavy string manipulation during the core search loop.

## Files
- `day13.c` - Main C source code with seating solver
- `Makefile` - Build configuration for cc65
