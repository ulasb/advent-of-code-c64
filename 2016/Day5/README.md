# Advent of Code 2016 - Day 5 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 5, written in C and compiled with cc65.

## The Problem

**Day 5: How About a Nice Game of Chess?**

The door to Santa's workshop is protected by a password hash system.
- **Part 1:** Find an 8-character password. Each character is the 6th hex character of an MD5 hash that starts with five zeroes (`00000`). The hash is calculated from a `door_id` concatenated with an increasing `index`. ✅ **Implemented (Demo mode)**
- **Part 2:** Similar to Part 1, but the 6th character indicates the position (0-7) and the 7th character is the value. ⚠️ **Logic implemented, but execution is impractical on real hardware.**

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day5.prg`.

### Testing
To run it in VICE:
```bash
make run
```

The program will run a demonstration test using the example ID `abc`. 

**Note on Performance:** MD5 hashing is extremely CPU-intensive. On the C64's 1MHz 6502 processor, calculating millions of hashes is not feasible for real-time results. The demo is configured to stop after finding the first character to show the algorithm is working.

## Technical Notes

### MD5 on the 6502

The primary challenge of this day is the MD5 algorithm itself. MD5 involves 64 rounds of bitwise operations (AND, OR, XOR, NOT), additions, and left-rotations on 32-bit integers.

1. **32-Bit Math:** The 6502 is an 8-bit processor. Every 32-bit addition or shift takes many CPU cycles.
2. **Rotation:** `cc65` does not have a native 32-bit rotate instruction. Implementing `(x << n) | (x >> (32-n))` in C results in very slow code.
3. **Memory Pressure:** The MD5 context and internal buffers take up significant chunks of the limited C64 memory.

### C64-Specific Optimizations

- **Global Buffers:** To avoid stack overflows (the 6502 stack is only 256 bytes), large structures like `MD5_CTX` and the transformation block `global_x` are placed in the BSS segment (global scope).
- **conio.h UI:** We use `conio.h` for direct, fast screen output.
- **Progress Reporting:** A hash counter is displayed and updated every 1024 hashes to show system activity.
- **Input Construction:** Instead of using `sprintf` for every iteration, we use a global buffer and `strcat` to combine the `door_id` and `index`.

### Performance Estimates

- **Hash Rate:** Approx. 10-20 hashes per second (unoptimized C).
- **First Character (ID: abc):** Found at index `3,231,929`.
- **Estimated Time:** At 15 hashes/sec, the first character would take ~60 hours of continuous execution.

Because of this, the C64 version is primarily a **proof of concept** of the logic port.

## Files

- `day5.c` - Main source code for Day 5 puzzle logic.
- `md5.c`/`md5.h` - Shared MD5 implementation.
- `Makefile` - Build script for cc65.
- `README.md` - This file.
- `day5.prg` - Compiled C64 executable.

## Why This Port is Different from Python

The Python implementation uses `hashlib`, which calls highly optimized C or assembly libraries (like OpenSSL) that use modern CPU instructions (like AVX or specialized SHA/AES instructions). On the C64, we are doing it all "the hard way" on an 8-bit bus at 1 million cycles per second.

For practical solving of this puzzle, the Python version remains the recommended tool:
https://github.com/ulasb/advent-of-code-python/tree/main/2016/day_5/
