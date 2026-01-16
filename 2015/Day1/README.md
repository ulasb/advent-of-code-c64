# Advent of Code 2015 - Day 1 (C64 Edition)

![Day 1 Image](https://github.com/ulasb/advent-of-code-c64/blob/main/2015/Day1/day1_screenshot.png)

This is a Commodore 64 implementation of Advent of Code 2015 Day 1, written in C and compiled with cc65. You can get more information, a complete problem definition, and a larger input file on [Advent of Code](http://www.adventofcode.com).

## The Problem

Santa is trying to deliver presents in a large apartment building, but he needs to find the right floor. The building uses a strange system where:
- `(` means go up one floor
- `)` means go down one floor

### Part 1
Find the floor Santa ends up on after following all the instructions.

### Part 2
Find the position of the first character that causes Santa to enter the basement (floor -1).

## Running the Program

### Requirements
- cc65 compiler suite
- Commodore 64 emulator (like VICE) or real C64 hardware

### Compilation
```bash
make
```

This creates `santa_floors.prg` which can be loaded and run on a Commodore 64.

### Testing
The program includes all the test cases from the problem description:
- Part 1: 9 test cases covering floor calculations
- Part 2: 2 test cases covering basement position detection

All tests should pass with "(PASS)" indicators.

## Test Cases Included

### Part 1 Tests
- `(())` and `()()` both result in floor 0
- `(((` and `(()(()(` both result in floor 3
- `))(((((` also results in floor 3
- `())` and `))(` both result in floor -1
- `)))` and `)())())` both result in floor -3

### Part 2 Tests
- `)` causes basement entry at character position 1
- `()())` causes basement entry at character position 5

## Files
- `santa_floors.c` - Main C source code
- `Makefile` - Build configuration
- `santa_floors.prg` - Compiled Commodore 64 program
