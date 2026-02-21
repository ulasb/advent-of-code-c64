# Advent of Code 2016 - Day 23 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 23, written in C and compiled with cc65.

## The Problem

**Day 23: Safe Cracking**

We need to crack a safe using an upgraded version of the Assembunny interpreter from Day 12.
- **Part 1:** Calculate the value in register `a` when it starts at `7`. ✅ **Implemented**
- **Part 2:** Calculate the value in register `a` when it starts at `12`. ✅ **Implemented**

The interpreter now includes a `tgl` instruction that can modify other instructions in the program at runtime.

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day23.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day23.prg` manually into your emulator.

The program will run the puzzle for Part 1 and Part 2 and display results on screen. Press Enter to exit when complete.

## Implementation Details

### Assembunny Interpreter
The C implementation translates the 26 instructions into an internal structure. It follows the standard Assembunny rules:
- `cpy x y`: copies `x` (number or register) into register `y`.
- `inc x`: increases the value of register `x` by one.
- `dec x`: decreases the value of register `x` by one.
- `jnz x y`: jumps to an instruction `y` away (positive or negative) if `x` is not zero.
- `tgl x`: toggles the instruction `x` away from the current one.

### Dynamic Optimization
To handle Part 2 (`12!`), which would otherwise take billions of cycles (days of execution on a 1MHz C64), the interpreter includes a **peephole optimizer** that detects the multiplication idiom:

```
cpy b c
inc a
dec c
jnz c -2
dec d
jnz d -5
```

This pattern is automatically replaced with `a += b * d` at runtime, provided the registers haven't been toggled into something else.

### C64-Specific Challenges

#### 1. 32-bit Arithmetic
The C64's 6502 is an 8-bit processor. Calculating `12!` (~479 million) requires 32-bit integers. We use the `long` type in `cc65`, which provides signed 32-bit arithmetic.

#### 2. Memory and Stack
The program uses a fixed array for the 26 instructions to keep the BSS segment small. All large structures are global to avoid stack overflow issues common on the 6502.

#### 3. Output
We use `conio.h` for direct and reliable text output to the C64 screen.

## Files

- `day23.c` - Main source code and puzzle logic.
- `Makefile` - Build script for `cc65`.
- `README.md` - This file.
- `day23.prg` - Compiled C64 executable.

## Building from Scratch

```bash
make clean  # Remove all build artifacts
make        # Rebuild day23.prg
make run    # Compile and run in VICE
```
