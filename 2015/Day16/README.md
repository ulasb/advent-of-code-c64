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
- **Optimized Data Types**: The `Sue` struct uses `signed char` (1 byte) for properties instead of `int`. This saves 10 bytes per instance, which is significant for the full list of 500 Sues.
- **8-bit Arithmetic**: Loop counters and indices use `unsigned char` to take advantage of the 6502's efficient 8-bit registers.
- **Total memory usage**: Extremely low (<1KB data structure overhead), well within C64 limits.

### Implementation Details
- **X-Macros**: The program uses an X-macro (`PROPERTIES`) as a single source of truth for property names, display labels, target values, and matching rules. This reduces code duplication and ensures that adding a new property only requires a single line change.
- **Robust Initialization**: Test data is initialized by coupling array indexing with counter increments (`test_sues[count++]`), preventing off-by-one errors and ensuring data integrity during setup.
- **Compile-time Rules**: Part 2 matching rules (Equal, Greater Than, Less Than) are handled via token concatenation in the macro expansion, which eliminates branchy runtime checks and "constant comparison" compiler warnings.

### Algorithm
1. **Define Target**: Properties and their target values are centralized in the `PROPERTIES` macro.
2. **Iterate**: The program iterates through a sample set of Aunt Sue records.
3. **Check**:
   - **Part 1**: Checks for an exact match on all known properties.
   - **Part 2**: Implements range matching (cats/trees > target, pomeranians/goldfish < target) and exact matching for other properties.
4. **Result**: Validates each Sue against both parts and outputs a "PASS" or "NO MATCH" indicator.

## Files
- `day16.c` - Main C source code
- `Makefile` - Build configuration for cc65
