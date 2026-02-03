# Advent of Code 2016 - Day 16 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 16, written in C and compiled with cc65.

## The Problem

**Day 16: Dragon Checksum**

The task is to fill a disk of a given size with data generated using a modified dragon curve and then calculate a checksum.
- **Part 1:** Disk size 272. ✅ **Implemented**
- **Part 2:** Disk size 35,651,584. ⚠️ **Not implemented** (see Known Limitations)

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day16.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day16.prg` manually into your emulator.

The program will run automated tests and display results on screen. Press Enter to exit when complete.

## Test Cases Included

The implementation includes verification tests from the problem description:
- **Dragon Generation:** Verifies "1" -> "100", "0" -> "001", "11111" -> "11111000000", etc.
- **Checksum Calculation:** Verifies "110010110100" -> "110101".
- **Full Example:** Verifies initial state "10000" with disk size 20 results in checksum "01100".

## Known Limitations

### Part 2 Not Implemented

Part 2 requires a disk size of 35,651,584 bits (approx 35 MB). Since the Commodore 64 has only 64 KB of RAM, storing the entire generated string is impossible. While a streaming approach (generating bits on the fly using recursion) is theoretically possible, it would be extremely slow on a 1 MHz processor.

For Part 2 solutions, please refer to the Python implementation at: [https://github.com/ulasb/advent-of-code-python/tree/main/2016/day_16/](https://github.com/ulasb/advent-of-code-python/tree/main/2016/day_16/)

## Technical Notes

### Algorithm

1. **Dragon Generation:**
   - Modified dragon curve: `a + '0' + reverse(flip(a))`
   - Implemented using iterative expansion in a global buffer.
   
2. **Checksum:**
   - Pairs of characters: same -> '1', different -> '0'.
   - Repeated until the length is odd.

### C64-Specific Challenges and Solutions

1. **Memory Management:**
   - Large buffers are placed in global scope to avoid stack overflow.
   - Buffer size refined to support Part 1 requirements (~300 bytes).

2. **UI/Output:**
   - Uses `conio.h` for direct screen access, which is more reliable on C64 than standard `printf`.
   - Custom background and border colors for a premium feel.

3. **C89 Compliance:**
   - Code adheres to C89 standard as required by `cc65`.

## Files

- `day16.c` - Main source code
- `Makefile` - Build script
- `README.md` - This file
- `day16.prg` - Compiled C64 executable

---
*Implementation by Antigravity AI*
