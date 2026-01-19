# Advent of Code 2016 - Day 4 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 4, written in C and compiled with cc65.

## The Problem

**Day 4: Security Through Obscurity**

Finally, you come across the information you need: a list of rooms in the information bureau at the Easter Bunny HQ.
- **Part 1:** Validate room names based on letter frequency checksums and sum their sector IDs. ✅ **Implemented**
- **Part 2:** Decrypt the room names using a Caesar cipher (shifted by sector ID) to find the "northpole object storage" room. ✅ **Implemented**

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day4.prg`.

### Testing
To run it in VICE:
```bash
x64 day4.prg
```
The program includes integrated unit tests from the original problem description.

## Implementation Details

### Algorithm

**Room Validation (Part 1):**
1. Parse the room string to extract the encrypted name, sector ID, and checksum.
2. Count the frequency of each letter (a-z) in the name.
3. Sort the letters primarily by frequency (descending) and secondarily by alphabetical order (ascending) to handle ties.
4. Verify if the top 5 letters match the provided checksum.

**Decryption (Part 2):**
1. Use a Caesar cipher where each letter is shifted forward in the alphabet by the sector ID.
2. The shift amount is calculated as `sector_id % 26`.
3. Hyphens are replaced with spaces.

### C64-Specific Technical Notes

#### 1. Screen Output with conio.h
Standard `printf` behavior on the C64 can be inconsistent depending on the environment. This implementation uses `conio.h` for direct screen access, providing:
- Persistent output that doesn't disappear when the program finishes.
- `cprintf` with `\r\n` for reliable line endings.
- `clrscr()` for a clean UI at startup.

#### 2. C89 Compliance
The code adheres to the C89 standard as required by `cc65`, ensuring all variable declarations occur at the beginning of their respective scopes.

#### 3. Memory Management
Large constant string arrays (like the sample input) are stored in the data segment. The implementation avoids dynamic memory allocation (`malloc`), which is generally discouraged on the 1MHz 6502 processor due to overhead and fragmentation risks.

#### 4. Sorting
The standard `qsort` from `stdlib.h` is used for frequency sorting. While `cc65`'s `qsort` is not the fastest, for a 26-element array, it performs efficiently enough to be unnoticeable.

## Test Cases Included

The following test cases from the original puzzle are hardcoded and verified at runtime:
- `aaaaa-bbb-z-y-x-123[abxyz]` -> Valid (123)
- `a-b-c-d-e-f-g-h-987[abcde]` -> Valid (987)
- `not-a-real-room-404[oarel]` -> Valid (404)
- `totally-real-room-200[decoy]` -> Invalid
- `qzmt-zixmtkozy-ivhz-343[zimth]` -> Decrypts to "very encrypted name"

## Files

- `day4.c`: Main source code.
- `Makefile`: Build script for the `cc65` toolset.
- `README.md`: This documentation.
- `day4.prg`: Compiled C64 executable.

## Reference

- Python solution: [ulasb/advent-of-code-python/2016/day_04/](https://github.com/ulasb/advent-of-code-python/tree/main/2016/day_04/)
