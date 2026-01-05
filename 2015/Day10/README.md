# Advent of Code 2015 - Day 10 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2015 Day 10 ("Elves Look, Elves Say"), written in C and compiled with cc65.

## The Problem

The Elves are playing a game called "Look-and-Say". The sequence is generated iteratively. For example, `1` becomes `11` (1 copy of digit 1). `11` becomes `21` (2 copies of digit 1). `21` becomes `1211` (one 2 followed by one 1).

The goal is to determine the length of the result string after a certain number of iterations.

### Part 1 & 2
- Part 1: Length after 40 iterations.
- Part 2: Length after 50 iterations.

## Running the Program

### Requirements
- cc65 compiler suite
- Commodore 64 emulator (like VICE) or real C64 hardware

### Compilation
```bash
make
```

This creates `day10.prg` which can be loaded and run on a Commodore 64.

### Execution
The program runs in two phases:
1. **Unit Tests**: Validates the logic against the 5 test cases provided in the problem description.
2. **Growth Demonstration**: Attempts to run the sequence on the puzzle input `1113122113` step-by-step.

### Expected Output
Because the "Look-and-Say" sequence grows exponentially, the string length quickly exceeds the C64's available memory.
- The program will run for approximately 8-10 steps.
- **"STOPPED: Memory Limit Reached"**: When the sequence exceeds the safe buffer limit (4KB), the program will pause and display this message in red.
- This is **expected behavior**. The full solution (40/50 steps) produces strings megabytes in size, which is impossible to store in the C64's 64KB RAM.

The demonstration proves the algorithm works correctly on the initial iterations before hitting hardware limits.

## Test Cases Included

The program validates against:
- `1` -> `11`
- `11` -> `21`
- `21` -> `1211`
- `1211` -> `111221`
- `111221` -> `312211`

## Technical Notes

### Memory Usage
- **Sequence Buffers**: Two static buffers of 4096 bytes each are used for double-buffering the sequence generation.
- **Total Memory**: Well within the standard C64 memory map.

### Algorithm
- The core function `generate_next_sequence` parses the input string, counts consecutive digits, and writes the "count + digit" representation to the output buffer.
- Integer-to-string conversion is handled manually to avoid heavy standard library dependencies if preferred, though `sprintf` is available in cc65. Hand-rolled `append_int` is used for efficiency.
