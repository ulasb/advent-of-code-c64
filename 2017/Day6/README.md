# Advent of Code 2017 - Day 6 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2017 Day 6, written in C and compiled with cc65.

## The Problem

**Day 6: Memory Reallocation**

A debugger program here is having an issue: it is trying to repair a memory reallocation routine, but it keeps getting stuck in an infinite loop.
- **Part 1:** Given the initial block counts in each memory bank, how many redistribution cycles must be completed before a configuration is produced that has been seen before? ✅ **Implemented**
- **Part 2:** Out of those cycles, how many cycles are in the infinite loop that arises from the configuration in question? ✅ **Implemented**

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day6.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day6.prg` manually into your emulator.

The program will run automated tests and display results on screen. Press Enter to exit when complete.

## Test Cases Included

Since the C64 has limited memory, storing all historical states for the full puzzle input (which typically takes several thousands of cycles) would quickly exhaust the available BSS segment memory on the 6502 architecture.

As per the requirements, we do not use an input file but provide the sample case as a proof of concept.

### Tests
- **Input:** `0 2 7 0`
  - **Part 1 Expected:** 5 cycles ✅
  - **Part 2 Expected:** 4 cycles loop size ✅

## Technical Notes

### Algorithm

**Memory Redistribution Strategy:**
- The algorithm repeatedly redistributes blocks from the memory bank with the highest number of blocks.
- Ties are won by the lowest-numbered memory bank.
- Blocks are redistributed sequentially to the following banks.
- After each cycle, the complete state of the memory banks is saved.
- We check historical states at each cycle; if a duplicate state is found, we stop. `Part 1` is the current cycle count, and `Part 2` is the difference between the current cycle and when the state was first seen.
- Complexity per cycle: O(n × c) where n = number of banks and c = cycles occurred so far (for duplicate search).

### C64-Specific Limitations and Solutions

#### 1. Stack Limitations
The C64's 6502 processor has a 256-byte hardware stack.
**Solution:** `cc65` enforces strict limits, so large allocations (like `states` array) are moved to the global BSS segment.

#### 2. Memory Constraints
Full puzzle input takes thousands of cycles, requiring hundreds of kilobytes to store states. The C64 only has 64KB total RAM.
**Solution:** The history array is aggressively bound (`MAX_STATES 500`) and the bank elements use single bytes (`unsigned char`) to fit within reasonable memory limits. This allows the proof of concept to comfortably evaluate the sample case and inputs that converge quickly.

#### 3. Output Reliability
`printf` from `stdio.h` can be unstable. Thus, `conio.h` and the C64 specific rendering features are used. Programs wait for user input (`cgetc()`) to prevent the C64 from dropping back to a BASIC prompt right after completion.

## Files

- `day6.c` - Main source code (Part 1 and 2 implemented)
- `Makefile` - Build script for cc65
- `README.md` - This file
