# Advent of Code 2015 - Day 9 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2015 Day 9, written in C and compiled with cc65. You can get more information, a complete problem definition, and a larger input file on [Advent of Code](http://www.adventofcode.com).

## The Problem

Every year, Santa manages to deliver all of his presents in a single night. This year, however, he has some new locations to visit; his elves have provided him the distances between every pair of locations. He can start and end at any two (different) locations he wants, but he must visit each location exactly once. What is the shortest distance he can travel to achieve this?

### Part 1
Given a list of distances between cities, find the shortest possible route that visits each city exactly once.

### Part 2
Given the same distances, find the longest possible route that visits each city exactly once.

## Running the Program

### Requirements
- cc65 compiler suite
- Commodore 64 emulator (like VICE) or real C64 hardware

### Compilation
```bash
make
```

This creates `day9.prg` which can be loaded and run on a Commodore 64.

### Testing
The program includes the test cases specified for the example input.

### Example Data
- London to Dublin = 464
- London to Belfast = 518
- Dublin to Belfast = 141

### Expected Results
- **Shortest Route**: London -> Dublin -> Belfast = 605
- **Longest Route**: Dublin -> London -> Belfast = 982

## Technical Notes

### Memory Usage
- **City storage**: Fixed size arrays for city names and distance matrix.
- **Recursive Solver**: Uses stack depth equal to number of cities (minimal).
- **Total memory usage**: Very low (~few KB), well within C64 limits.

### Data Types
- **Distances**: `unsigned int` (16-bit) to handle summed distances (max route < 65535 for typical inputs).
- **City Names**: Fixed-size character arrays.

### Algorithm Implementation

The program implements a variation of the Traveling Salesperson Problem (TSP) solver:

1. **Modeling**: Cities are mapped to indices 0..N-1. Distances are stored in an adjacency matrix.
2. **Permutation**: We generate all permutations of cities to represent all possible paths.
3. **Calculation**: For each permutation, we calculate the total distance.
4. **Optimization**: We track the minimum and maximum distances found.


### Constraints and Performance
The `MAX_CITIES` constant is set to 10 due to the **O(N!)** factorial complexity of the brute-force TSP algorithm used here. On a 1MHz 6510 CPU:
- **8 cities**: ~40,320 permutations (Runs in < 1 second).
- **10 cities**: ~3.6 million permutations (would take several minutes).
- **12+ cities**: Hundreds of millions of permutations (would take hours).

This limit safely covers the typical Advent of Code input (7-8 cities) while ensuring the program remains responsive on 8-bit hardware.

## Files
- `day9.c` - Main C source code with TSP solver
- `Makefile` - Build configuration for cc65
