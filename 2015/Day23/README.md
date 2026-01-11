# Advent of Code 2015 - Day 23 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2015 Day 23, written in C and compiled with cc65.

## The Problem

**Day 23: Opening the Turing Lock**

We've found a lock that can be opened by running a specific set of instructions on a simple virtual machine. The VM has two registers, `a` and `b`, and supports the following instructions:

- `hlf r`: Set register `r` to half its current value (integer division).
- `tpl r`: Set register `r` to triple its current value.
- `inc r`: Increment register `r` by 1.
- `jmp offset`: Jump relative to the current instruction pointer.
- `jie r, offset`: Jump if register `r` is even.
- `jio r, offset`: Jump if register `r` is exactly 1 (Jump If One).

### Parts
- **Part 1:** What is the value in register `b` after the program executes with initial values `a=0, b=0`? ✅ **Implemented**
- **Part 2:** What is the value in register `b` after the program executes with initial values `a=1, b=0`? ✅ **Implemented**

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

The program will run the instructions and display the final values of registers `a` and `b` for both Part 1 and Part 2. Press any key to exit when complete.

## Implementation Details

### Instruction Decoding

The instructions are represented as a structure:
```c
typedef struct {
    unsigned char op;
    unsigned char reg;
    int offset;
} Instruction;
```

### Register Handling

Register values can grow significantly depending on the input program. To ensure compatibility with potential large values, the registers are defined as `unsigned long` (32-bit in `cc65`).

### C64-Specific Optimizations

1. **Global State:** All register and instruction data are stored in the BSS segment (global scope) to avoid stack overflow issues common on the 6502 architecture.
2. **Direct Screen I/O:** The program uses `conio.h` for fast, reliable screen output, bypassing the slower `stdio.h` calls.
3. **C89 Compliance:** The code adheres to strict C89 standards as required by the `cc65` compiler.

## Technical Notes

### The "JIO" Pitfall
A common mistake in this puzzle is interpreting `jio` as "Jump If Odd". However, the problem specification defines it as **"Jump If One"**. The implementation correctly checks `reg == 1`.

### Performance
The virtual machine is extremely fast on the C64. Even with 32-bit register operations (which are emulated in software on the 8-bit 6502), the execution completes instantly for standard programs.

## Files

- `day23.c`: Main source code containing the VM and the instruction set.
- `Makefile`: Build script for `cc65`.
- `README.md`: This file.
- `day23.prg`: Compiled C64 executable.

---
*Created as part of the Advent of Code 2015 C64 Port project.*
