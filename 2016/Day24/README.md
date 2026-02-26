# Advent of Code 2016 - Day 24 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 24, written in C and compiled with cc65.

## The Problem

**Day 24: Air Duct Spelunking**

A robot needs to clean an HVAC system (a grid of walls `#` and open paths `.`). There are numbered points (0, 1, 2, ...) that must be visited.

- **Part 1:** What is the fewest number of steps required to visit every non-zero number starting from 0? ✅ **Implemented**
- **Part 2:** What is the fewest number of steps required to visit every non-zero number and then return to 0? ✅ **Implemented**

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day24.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day24.prg` manually into your emulator.

The program will run the example case and display results on screen. Press Enter to exit when complete.

## Test Cases Included

Since the C64 has limited memory and I/O capabilities, we use hardcoded test cases derived from the problem description to verify the solution logic.

### Example Case
- **Input:**
  ```
  ###########
  #0.1.....2#
  #.#######.#
  #4.......3#
  ###########
  ```
- **Part 1 Expected:** 14 steps ✅
- **Part 2 Expected:** 20 steps ✅

## Technical Notes

### Algorithm

1. **Grid Parsing:** The program identifies the (x, y) coordinates of each numbered point.
2. **All-Pairs Shortest Paths:** For each pair of numbered points, a **Breadth-First Search (BFS)** is executed to find the shortest distance through the maze.
3. **Traveling Salesperson Problem (TSP):**
   - The program uses a recursive permutation generator to explore all possible paths through the numbered points.
   - For Part 1, it calculates the minimum path length starting from '0'.
   - For Part 2, it adds the distance back to '0' from the last point.
   - For *N* points, it explores $(N-1)!$ permutations. For the typical puzzle size (8 points), this is $7! = 5040$ paths, which the C64 handles in reasonable time.

### C64-Specific Challenges and Solutions

#### 1. Memory Management
Large data structures are moved to global scope (BSS segment) to avoid stack overflow:
- `grid[40][181]`: Storage for the maze.
- `visited[40][180]`: BFS state tracking.
- `distances[8][8]`: Precomputed distance matrix.

#### 2. BFS Efficiency
- The BFS uses a circular queue to avoid expensive re-allocations.
- `memset` is used to quickly reset the `visited` array between BFS runs.

#### 3. Recursion
- `cc65` handles shallow recursion (like our 7-level deep TSP) well, but parameter counts are kept low to minimize software stack usage.

#### 4. UI Reliability
- Uses `conio.h` for direct, flicker-free status updates.

## Files

- `day24.c` - Main source code (BFS and TSP implementation)
- `Makefile` - Build script for cc65
- `README.md` - This file
- `day24.prg` - Compiled C64 executable

---

For the full Python implementation and the original puzzle input, please refer to:
https://github.com/ulasb/advent-of-code-python/tree/main/2016/day_24/
