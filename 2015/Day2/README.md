# Advent of Code 2015 - Day 2 (C64 Edition)

![Screenshot](https://github.com/ulasb/C64-Development/blob/main/Advent-of-Code/2015/Day2/present_wrapping.png)

This is a Commodore 64 implementation of Advent of Code 2015 Day 2, written in C and compiled with cc65. You can get more information, a complete problem definition, and a larger input file on [Advent of Code](http://www.adventofcode.com).

## The Problem

The elves are running low on wrapping paper, and so they need to submit an order for more. They have a list of the dimensions (length l, width w, and height h) of each present, and only want to order exactly as much as they need.

### Part 1
Find how much wrapping paper they need to order. The wrapping paper required for each present is the surface area of the present plus the area of the smallest side (for slack).

### Part 2
Find how much ribbon they need to order. The ribbon required for each present is the shortest distance around its sides (the smallest perimeter of any one face) plus the volume of the present (for the bow).

## Running the Program

### Requirements
- cc65 compiler suite
- Commodore 64 emulator (like VICE) or real C64 hardware

### Compilation
```bash
make
```

This creates `present_wrapping.prg` which can be loaded and run on a Commodore 64.

### Testing
The program includes all the test cases from the problem description:
- Part 1: 2 test cases covering wrapping paper calculations
- Part 2: 2 test cases covering ribbon length calculations

All tests should pass with "(PASS)" indicators.

## Test Cases Included

### Part 1 Tests (Wrapping Paper)
- A present with dimensions 2x3x4 requires 58 square feet of wrapping paper
- A present with dimensions 1x1x10 requires 43 square feet of wrapping paper

### Part 2 Tests (Ribbon)
- A present with dimensions 2x3x4 requires 34 feet of ribbon
- A present with dimensions 1x1x10 requires 14 feet of ribbon

## Files
- `present_wrapping.c` - Main C source code
- `Makefile` - Build configuration
- `present_wrapping.prg` - Compiled Commodore 64 program
