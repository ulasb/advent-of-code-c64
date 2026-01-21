# Advent of Code 2016 - Day 7 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 7, written in C and compiled with cc65.

## The Problem

**Day 7: Internet Protocol Version 7**

- **Part 1:** Determine if an IP address supports Transport-Layer Snooping (TLS). An IP supports TLS if it has an ABBA outside square brackets and no ABBA inside any square brackets.
- **Part 2:** Determine if an IP address supports Super-Secret Listening (SSL). An IP supports SSL if it contains an ABA pattern outside square brackets and a corresponding BAB pattern inside square brackets.

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64sc`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day7.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day7.prg` manually into your emulator (e.g., `x64sc day7.prg`).

The program will run automated tests and display results on screen. Press Enter to exit when complete.

## Implementation Details

### Algorithm

**Part 1 (TLS):**
- Scan the string character by character.
- Track whether we are currently inside or outside square brackets.
- Check for ABBA patterns (four characters `a b b a` where `a != b`).
- If an ABBA is found inside brackets, the IP is immediately disqualified.
- If an ABBA is found outside brackets, the IP is marked as potentially supporting TLS.
- Return true only if at least one ABBA was found outside and none were found inside.

**Part 2 (SSL):**
- Two-pass approach (or optimized single pass):
  1. Find all `a b a` patterns outside brackets (`a != b`) and store them.
  2. Find all `b a b` patterns inside brackets and check if the corresponding `a b a` was found outside.
- Alternatively: Store all ABAs found outside, then check all BABs found inside against the stored ABAs.
- On C64, we use a simple bitmask or fixed-size array to store found ABAs to minimize memory usage. Since ABA patterns use two characters (a and b), there are 26*26 = 676 possible ABAs. A bitset of 676 bits (85 bytes) is sufficient.

### C64-Specific Optimizations

1.  **Memory Management:** Large buffers and state arrays are moved to global scope (BSS segment) to avoid stack overflow.
2.  **UI:** Using `conio.h` for reliable screen output and colors.
3.  **String Handling:** Minimal use of heavy library functions; custom scanning logic for performance.

## Files

- `day7.c`: Main source code containing the logic and test data.
- `Makefile`: Build script for cc65.
- `README.md`: This file.
- `day7.prg`: Compiled C64 executable.
