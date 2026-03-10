# Advent of Code 2017 - Day 4 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2017 Day 4, written in C and compiled with cc65.

## The Problem

**Day 4: High-Entropy Passphrases**

A new system policy has been put in place for passphrases.
- **Part 1:** A passphrase is valid if it contains no duplicate words. ✅ **Implemented**
- **Part 2:** A passphrase is valid if it contains no words that are anagrams of each other. ✅ **Implemented**

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day04.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day04.prg` manually into your emulator.

The program will run automated tests and display results on screen. Press Enter to exit when complete.

## Test Cases Included

Since the C64 has limited memory and I/O capabilities, we use hardcoded test cases from the problem description to verify the solution logic.

### Part 1 Tests
- `aa bb cc dd ee` -> Valid
- `aa bb cc dd aa` -> Invalid (duplicate 'aa')
- `aa bb cc dd aaa` -> Valid

### Part 2 Tests
- `abcde fghij` -> Valid
- `abcde xyz ecdab` -> Invalid (anagram)
- `a ab abc abd abf` -> Valid
- `oiii ioii iioi` -> Invalid (anagram)

## Technical Notes

### Algorithm

**Passphrase Validation:**
- The input string is split into individual words.
- Words are stored in a global array to avoid stack overflow on the C64.
- **Part 1:** Performs a nested loop comparison to check for identical strings.
- **Part 2:** Sorts the characters within each word (using selection sort) before performing the nested loop comparison. Anagrams will result in identical sorted strings.

### C64-Specific Challenges and Solutions

#### 1. Stack Limitations
The C64's 6502 processor has a very small hardware stack. Large local variables are avoided by using global buffers.

#### 2. Output Reliability
The program uses `conio.h` for direct screen access, which is more reliable than `stdio.h` on the C64 platform.

#### 3. Memory Constraints
Buffer sizes are capped (e.g., `MAX_WORDS`, `MAX_WORD_LEN`) to fit within the C64's memory map.

---

For the original Python implementation, see: https://github.com/ulasb/advent-of-code-python/tree/main/2017/day_04/
