# Advent of Code 2016 - Day 21 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 21, written in C and compiled with cc65.

## The Problem

**Day 21: Scrambled Letters and Hash**

The task is to scramble and unscramble a password based on a series of specific string manipulation operations.

- **Part 1:** Apply a series of transformations to an initial password.
- **Part 2:** Reverse the transformations applied to a scrambled password to find the original.

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day21.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day21.prg` manually into your emulator.

The program will run automated tests for each transformation type and display the results. Press Enter to exit when complete.

## Implementation Details

### Handled Operations
1. **Swap Position X with Y:** Swaps characters at given indices.
2. **Swap Letter X with Y:** Swaps all occurrences of two characters.
3. **Rotate Left/Right X Steps:** Cyclic shift of the password.
4. **Rotate Based on Letter Position:** Rotates right by `1 + index + (index >= 4 ? 1 : 0)`.
5. **Reverse Positions X through Y:** Reverses a substring in place.
6. **Move Position X to Y:** Removes character at X and inserts it at Y.

### Reverse Operations
For Part 2, reversing instructions requires inverting each step:
- Swap (Position/Letter) is its own inverse.
- Rotate Left becomes Rotate Right (and vice versa).
- Move X to Y becomes Move Y to X.
- **Undo Rotate Based on Position:** Implemented using a brute-force search over all possible rotations (8 for an 8-character string), finding the one that produces the current state when rotated forward.

## Technical Notes

### C64 Optimization
- **Memory Management:** Uses global buffers (`current_password`, `temp_buffer`) to avoid stack overflow on the 6502's limited stack.
- **Screen I/O:** Utilizes `conio.h` for direct screen access, which is more reliable than standard `printf` on the C64.
- **String Operations:** Uses `memmove` and `memcpy` for efficient character shifts and copies.

### C89 Compliance
- All variables are declared at the beginning of their respective blocks to satisfy `cc65`'s C89 requirements.

## Files
- `day21.c`: Main source code containing the transformation engine and unit tests.
- `Makefile`: Build script for the `cc65` compiler.
- `README.md`: project documentation.
- `day21.prg`: Compiled Commodore 64 executable.
