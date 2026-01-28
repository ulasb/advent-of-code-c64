# Advent of Code 2016 - Day 10 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 10, written in C and compiled with cc65.

## The Problem

**Day 10: Balance Bots**

Bots are moving microchips around. Each bot can hold up to two chips. When a bot has two chips, it compares them and gives the lower-valued chip to one destination and the higher-valued chip to another.

- **Part 1:** Find the ID of the bot that compares chips with values 17 and 61 (or custom values for tests). ✅ **Implemented**
- **Part 2:** Calculate the product of the values of the chips deposited in output bins 0, 1, and 2. ✅ **Implemented**

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator

### Compilation
To compile the program, run:
```bash
make
```
This produces `day10.prg`.

### Testing
To run it in VICE:
```bash
make run
```
The program runs automated tests based on the problem's sample input and then executes the logic. Press Enter to exit when complete.

## Test Cases Included

The program uses the following sample input for validation:
```
value 5 goes to bot 2
bot 2 gives low to bot 1 and high to bot 0
value 3 goes to bot 1
bot 1 gives low to output 1 and high to bot 0
bot 0 gives low to output 2 and high to output 0
value 2 goes to bot 2
```

**Expected Results for Sample:**
- Bot 2 compares 2 and 5.
- Bot 1 compares 2 and 3.
- Bot 0 compares 3 and 5.
- Output 0: 5
- Output 1: 2
- Output 2: 3
- Product (0*1*2): 30

## Technical Notes

### C64-Specific Optimizations
1. **Global Data Structures:** To avoid stack overflow on the 6502's 256-byte stack, all bot and output data are stored in global arrays.
2. **Queue-based Processing:** Bots that receive their second chip are added to a queue for processing, avoiding recursive calls.
3. **conio.h:** Used for fast and reliable screen output.
4. **Memory Management:** Fixed-size arrays are used instead of dynamic allocation to ensure stability within the 64KB RAM limit.

### Data Structures
```c
typedef struct {
    int chips[2];
    int chip_count;
    char low_type;   // 0 for Bot, 1 for Output
    int low_id;
    char high_type;
    int high_id;
    char defined;    // Flag to check if directions are set
} Bot;
```

## Files
- `day10.c` - Main source code
- `Makefile` - Build script
- `README.md` - This file
