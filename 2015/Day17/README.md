# Advent of Code 2015 - Day 17: No Such Thing as Too Much

This directory contains the C64 solution for finding combinations of containers to fit a specific amount of eggnog.

## Problem Description
The elves bought too much eggnog (e.g., 150 liters) and need to fit it into containers. 
- **Part 1**: Find the number of different combinations of containers that exactly match the target volume.
- **Part 2**: Find the number of ways to fill the minimum number of containers.

## Implementation Details
The solution uses a recursive depth-first search (DFS) to explore all combinations of containers.
- **Language**: C (compiled with cc65)
- **Target**: Commodore 64

The input is currently hardcoded for the sample case:
- target: 25
- containers: 20, 15, 10, 5, 5

Expected results for sample:
- Part 1: 4
- Part 2: 3 (min containers: 2)

To use the real input, update `containers[]` and `target` variables in `main.c`.

## Building and Running

### Requirements
- **cc65**: A cross-compiler for 6502-based systems (like C64).
- **VICE**: A Commodore emulator (optional, for running).

### Instructions
1. Compile the code:
   ```sh
   make
   ```
   This produces `day17.prg`.

2. Run with VICE (x64):
   ```sh
   x64 day17.prg
   ```
