# Advent of Code 2016 - Day 14 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 14, written in C and compiled with cc65.

## The Problem

**Day 14: One-Time Pad**

We need to generate keys for a one-time pad by hashing a salt followed by an increasing index.
- **Part 1:** Find the 64th key using standard MD5 hashing. ✅ **Implemented**
- **Part 2:** Find the 64th key using key stretching (MD5 hashed 2017 times). ⚠️ **Implementation included but extremely slow** (see Performance)

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator

### Compilation
To compile the program, run:
```bash
make
```
This produces `day14.prg`.

### Testing
To run it in VICE:
```bash
make run
```

The program will run with a default salt and find the 64th key.

## Technical Notes

### Algorithm optimization
MD5 is computationally expensive for the 6502 processor (1 MHz). To make this viable:
1. **Sliding Window:** We maintain a 1000-hash lookahead window.
2. **Compact Storage:** Instead of storing 1000 full hex strings (32KB), we only store the first triplet character and a bitmask of quintet characters (3 bytes per hash).
3. **Optimized MD5:** A C-based MD5 implementation tuned for 8-bit systems.

### Performance (CRITICAL NOTE)
**This implementation is extremely slow on real hardware.** 
- **Part 1:** On a standard 1MHz C64, an MD5 hash takes nearly 1 second. Finding all 64 keys involves hashing tens of thousands of indices, which can take **several hours**. It is highly recommended to run this in an emulator with "Warp Mode" enabled.
- **Part 2:** This is **2017 times slower** than Part 1. Finding even a single key can take days of real time. This mode is included for logic completeness but is completely impractical without a SuperCPU or extreme emulation acceleration.

## Files
- `day14.c` - Main source code including MD5 and puzzle logic
- `Makefile` - Build script
- `README.md` - This file
