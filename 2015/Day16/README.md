# Advent of Code 2015 - Day 16 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2015 Day 16, written in C and compiled with cc65.

## The Problem

Your Aunt Sue has given you a wonderful gift, but you can't remember which Aunt Sue it was! You have a list of 500 Aunt Sues and their properties (children, cats, samoyeds, pomeranians, akitas, vizslas, goldfish, trees, cars, perfumes). You also have a "ticker tape" (the forensic target values) of the things you remember about the correct Aunt Sue.

### Part 1
Find the Aunt Sue whose known properties match the target values exactly. Missing properties are assumed to match.

### Part 2
The retrospective analysis reveals that the ticker tape data for cats and trees implies "greater than", while pomeranians and goldfish imply "fewer than". All other compounds imply "equal to". Find the Sue that matches these new rules.

## Running the Program

### Requirements
- cc65 compiler suite
- Commodore 64 emulator (like VICE) or real C64 hardware

### Compilation
```bash
make
```

This creates `day16.prg` which can be loaded and run on a Commodore 64.

### Testing
Since there is no input file for this C64 specific implementation, the program generates a sample set of Aunt Sue data to validate the algorithm against both Part 1 and Part 2 logic.

## Technical Notes

### Memory Usage
- **Data Structures**: The `Sue` struct uses `int` (2 bytes) for each property. 
- **Storage**: We process a small sample set in memory.
- **Total memory usage**: Extremely low (<1KB), well within C64 limits.

### Algorithm
1. **Define Target**: The target properties are stored as constants.
2. **Iterate**: We assume a list of Sues (simulated in the `run_tests` function).
3. **Check**:
   - **Part 1**: Checks equality for all defined properties.
   - **Part 2**: Checks ranges (> or <) for specific properties and equality for others.
4. **Result**: The program outputs PASS/FAIL for each sample Sue.

## Files
- `day16.c` - Main C source code
- `Makefile` - Build configuration for cc65
