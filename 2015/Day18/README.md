# Advent of Code 2015 - Day 18

This project contains a C64 solution for [Advent of Code 2015 Day 18](https://adventofcode.com/2015/day/18), written in C and compiled using `cc65`.

## Description

The goal is to simulate a grid of lights (Game of Life style) and count how many are on after 100 steps.
- **Part 1**: Standard Game of Life rules.
- **Part 2**: The four corners are stuck "ON".

## Project Structure

- `main.c`: The main C source code.
- `Makefile`: Build script for `cl65`.
- `input.txt`: The input grid (optional).

## Building and Running

1.  Make sure `cc65` is installed and in your PATH.
2.  Run `make` to build the `day18.prg` executable.
3.  Run `day18.prg` in a C64 emulator (like VICE).

```bash
make
x64sc -autostart day18.prg
```

## Input Handling

The program checks for `input.txt`. If found, it runs the simulation on the file's grid for 100 steps.
If not found, it runs the example 6x6 grid from the problem description for 4/5 steps.

## Performance

The grid size is supported up to 100x100.
The simulation uses `unsigned char` arrays and sentinel borders to optimize neighbor counting by avoiding boundary checks in the inner loops.
