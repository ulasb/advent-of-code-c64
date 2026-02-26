# Advent of Code 2016 - Day 25: Clock Signal

## Commodore 64 Version

This is a C implementation of the Advent of Code 2016 Day 25 puzzle, designed to be compiled for the Commodore 64 using the `cc65` toolchain.

### The Problem

The goal is to find the smallest non-negative integer for register `a` that, when used as input to an "Assembunny" program, produces an infinite clock signal of alternating `0` and `1` values.

### Implementation Details

- **Interpreter**: A custom Assembunny interpreter that handles `cpy`, `inc`, `dec`, `jnz`, and `out` instructions.
- **Optimization**: Includes a hardcoded optimization for the multiplication idiom (looping `inc` and `dec` across two registers), which is essential for performance on the 6502 processor.
- **Verification**: The program tests each value of `a` and verifies that the first 30 outputs follow the `0, 1, 0, 1...` pattern.

### Requirements

- [cc65](https://cc65.github.io/) toolchain
- A Commodore 64 emulator (like VICE) to run the `.prg` file.

### Building and Running

To compile the project:

```bash
make
```

To run it (using `x64` emulator):

```bash
make run
```

### Project Structure

- `day25.c`: Main source code containing the interpreter and search logic.
- `Makefile`: Build script for `cl65`.
- `README.md`: Project documentation.
