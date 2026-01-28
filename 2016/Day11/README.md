# Advent of Code 2016 - Day 11 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 11, written in C and compiled with cc65.

## The Problem

**Day 11: Radioisotope Thermoelectric Generators**

We need to move several generators and microchips to the top floor of a facility.
- **Part 1:** Move 5 pairs of items (generators and microchips) to the 4th floor. ✅ **Implemented**
- **Part 2:** Move 7 pairs of items (adding 2 new pairs) to the 4th floor. ⚠️ **Not implemented** (see Known Limitations)

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day11.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day11.prg` manually into your emulator.

The program will run automated tests (including the example from the puzzle) and display results on screen. Press Enter to exit when complete.

## Test Cases Included

### Example Test
- **Input:** 
  - Floor 1: Hydrogen-compatible microchip, Lithium-compatible microchip
  - Floor 2: Hydrogen generator
  - Floor 3: Lithium generator
- **Expected:** 11 steps ✅

### Part 1
- **Input:** Hardcoded from `input.txt` (5 pairs)
- **Result:** Calculated by the program.

## Technical Notes

### Algorithm

**BFS with Symmetry Breaking:**
- The puzzle has many equivalent items (interchangeable pairs).
- To reduce the state space, we use a **canonical state representation** where pairs are sorted.
- Instead of hashing or large state arrays, we map the canonical state to a unique integer index:
  - `elevator_pos` (0-3)
  - `combination_index` (mapping sorted 5 pairs across 16 possible (G,C) floor combinations).
- This results in exactly 62,016 possible canonical states for 5 pairs.
- We use a **bitset** of 62,016 bits (7.5 KB) to track visited states.
- BFS is performed level-by-level using a global queue to manage memory.

### C64-Specific Challenges

#### 1. Memory Management
The C64 has 64KB RAM. We use:
- **Bitset:** 7.5 KB for visited states.
- **Queue:** 15 KB (2500 entries * 6 bytes per state).
- **Lookup Table:** Precomputed combinatorial coefficients (`choose[n][k]`).
- Total memory usage is well within the 64KB limit for Part 1.

#### 2. Efficiency
- `cc65` is used with `-O` optimization.
- Global arrays are used to avoid stack overflow.
- State packing and unpacking are optimized for the 6502's limited register set where possible.

### Known Limitations

#### Part 2 Not Implemented
Part 2 adds 2 more pairs (7 pairs total). The number of canonical states grows to ~682,000, which would require an 83 KB bitset. This exceeds the total RAM of the Commodore 64 (64 KB).

**Why not use A*?**
While A* explores fewer states than BFS, its memory overhead per state is much higher. A hash map tracking 16,000 visited states would require ~192 KB for state data and pointers, which is far beyond the C64's capacity.

**What about IDA*?**
Iterative Deepening A* (IDA*) would solve the memory issue by using only a few hundred bytes for the current path (Depth-First Search). However, without a visited state set to prune cycles, the algorithm would re-explore states millions of times. On a 1 MHz 6502 processor, calculating Part 2 would likely take hours or even days.

For Part 2 solutions, please refer to the Python implementation at: https://github.com/ulasb/advent-of-code-python/tree/main/2016/day_11/

## Files
- `day11.c`: Main source code.
- `Makefile`: Build script for cc65.
- `README.md`: This file.
- `day11.prg`: Compiled C64 executable.
