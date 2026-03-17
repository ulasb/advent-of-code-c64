# Advent of Code 2017 - Day 5 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2017 Day 5, written in C and compiled with cc65.

## The Problem

**Day 5: A Maze of Twisty Trampolines, All Alike**

We need to help the CPU escape a maze of jump instructions.
- **Part 1:** Follow the jump instructions, incrementing each offset by 1 after using it.
- **Part 2:** If the offset was 3 or more, decrease it by 1 instead of increasing it.

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day5.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day5.prg` manually into your emulator.

The program will run an automated test based on the example in the problem description and display results on screen. Press Enter to exit when complete.

## Test Cases Included

We use a hardcoded sample case as a proof of concept (`{0, 3, 0, 1, -3}`).
- **Part 1 Expected:** 5 steps
- **Part 2 Expected:** 10 steps

## Files

- `day5.c` - Main source code
- `Makefile` - Build script for cc65
- `README.md` - This file
