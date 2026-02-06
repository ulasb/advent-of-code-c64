# Advent of Code 2016 - Day 17 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 17, written in C and compiled with cc65.

## The Problem

**Day 17: Two Steps Forward, One Step Back**

You find yourself in a 4x4 maze of rooms (0,0 to 3,3). 
Doors are locked or unlocked based on the MD5 hash of your passcode plus your current path ('U', 'D', 'L', 'R').
- **Part 1:** Find the shortest path to the vault at (3,3). ✅ **Implemented**
- **Part 2:** Find the length of the longest path that reaches the vault. ✅ **Implemented**

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64sc`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day17.prg`.

### Running
To run it in VICE:
```bash
make run
```
Or load `day17.prg` manually into your emulator.

The program will run tests for known passcodes and then solve for the puzzle input `pslxynzg`.

## Technical Implementation Notes

### Algorithms

#### Shortest Path (BFS)
We use a Breadth-First Search to find the shortest path.
- **Queue Management:** Given the C64's memory constraints, the queue size is limited to 100 states.
- **State Compression:** Each state stores (x, y, path_len) and the path string (up to 64 chars).

#### Longest Path (Iterative DFS)
Finding the longest path can involve paths over 800 steps long. 
- **Stack Safety:** Standard recursion would blow the C64 stack (which is limited and shared with the OS).
- **Manual Stack:** We implemented an iterative DFS using a manual stack (`DFSState dfs_stack[1000]`) in the BSS segment.
- **Backtracking:** The algorithm explores all paths and updates the maximum length found when reaching (3,3).

### MD5 Hashing Optimization
MD5 is computationally expensive on a 1MHz 6502.
- **Context Caching:** Since every hash starts with the same passcode, we pre-compute the MD5 context for the passcode and clone it for each move's hash calculation.
- **Faster Hex Conversion:** Used a lookup-table based conversion instead of `sprintf`.

### C64-Specific Challenges

1. **Memory (BSS) Management:** Total memory for the BFS queue, DFS stack, and MD5 buffers was carefully managed to fit within the 64KB RAM without overflowing the BSS segment.
2. **C89 Compliance:** Code follows strict C89 rules required by `cc65`.
3. **Screen I/O:** Used `conio.h` for reliable, persistent screen output.

## Files

- `day17.c` - Main source code
- `md5.c` / `md5.h` - Shared MD5 implementation
- `Makefile` - Build script
- `day17.prg` - Compiled executable

---
*Created as part of the Advent of Code C64 Porting project.*
