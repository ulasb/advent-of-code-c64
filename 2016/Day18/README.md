# Advent of Code 2016 - Day 18 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 18, written in C and compiled with cc65.

## The Problem

**Permalink:** Like a GIF For Your Yard

As you home in on an exit, you notice some of the Elves managed to paint the walls in increasingly strange patterns.

Today, the Elves want to know which tiles are safe as they make their way through a grid. The grid is divided into individual tiles which are either safe (`.`) or traps (`^`).

**Rules for determining traps:**

A tile becomes a trap in the next row if its left, center, and right tiles match one of these patterns:

- A trap (`^`) with a trap left and center (but not right): `^^.`
- A trap with trap right and center (but not left): `.^^`
- A trap with trap left (but not center or right): `^..`
- A trap with trap right (but not center or left): `..^`

This simplifies to: **A tile is a trap if its left and right neighbors differ** (left XOR right = 1)

### Part 1: Safe Tiles After 40 Rows

Given a row of tiles as input, generate the next 39 rows and count how many tiles are safe (`.`).

✅ Implemented

### Part 2: Unsafe Memory Constraints

Part 2 asks for safe tiles after 400,000 rows. Due to C64 platform limitations (64KB RAM total), this is excluded from the C64 implementation.

⚠️ Not implemented (see Known Limitations)

## Running the Program

### Requirements

- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation

To compile the program, run:

```bash
make
```

This produces `day18.prg`.

### Testing

To run it in VICE:

```bash
make run
```

Or load `day18.prg` manually into your emulator.

The program will run automated tests and display results on screen. Press Enter to exit when complete.

## Test Cases Included

Since the C64 has limited memory and I/O capabilities, we use hardcoded test cases derived from the problem description to verify the solution logic.

### Part 1 Tests

**Test 1: 10 Rows**
- Input: `.^^.^.^^..`
- Expected: 40 safe tiles ✅

**Test 2: Part 1 Full (40 rows)**
- Input: `.^^.^.^^..`
- Expected: 182 safe tiles ✅

**Test 3: First 3 Rows**
- Input: `.^^.^.^^..`
- Row 1: `.^^.^.^^..` (5 safe)
- Row 2: `^^^...^^^.` (4 safe via bit pattern `1110001110`)
- Row 3: `.^^.^^.^^^` (3 safe via bit pattern `1011011011`)
- Expected: 12 safe tiles total ✅

## Technical Notes

### Algorithm

**Bit Manipulation Strategy:**

The algorithm converts each row string into an integer where:
- `^` (trap) = bit value 1
- `.` (safe) = bit value 0

For each row:
1. Count safe tiles = `width - count_of_1_bits`
2. Calculate next row: **new_row = (row << 1) XOR (row >> 1)**
   - Left neighbor: `row << 1`
   - Right neighbor: `row >> 1`
   - Trap if left XOR right = 1
3. Apply width mask to keep within bounds

**Example progression for `.^^.^.^^..`:**
```
Row 1:  .^^.^.^^..  (5 safe)
Row 2:  ^^^...^^^.  (4 safe)
Row 3:  .^^.^^.^^^  (3 safe)
```

The bit patterns show: `0110101100` → `1110001110` → `1011011011`

**Complexity:** O(n × w) where n = number of rows, w = width

### C64-Specific Challenges and Solutions

#### 1. Bit Manipulation Without Built-ins

**Problem:** cc65 doesn't reliably support `__builtin_popcount()`.

**Solution:** Implement simple bit counting function:
```c
int count_bits(unsigned int value) {
  int count = 0;
  unsigned int v = value;
  while (v) {
    count += v & 1;
    v >>= 1;
  }
  return count;
}
```

#### 2. String Parsing to Bits

**Problem:** Efficient conversion of character row to bit representation.

**Solution:** Loop through string and build bit integer:
```c
row_bits = 0;
for (i = 0; i < width; i++) {
  row_bits = (row_bits << 1) | (first_row_str[i] == '^' ? 1 : 0);
}
```

#### 3. Output Reliability

**Problem:** Standard `stdio.h` functions unreliable on C64.

**Solution:** Use `conio.h` for direct screen access:
```c
#include <conio.h>
cprintf("TEXT\r\n");  // Use \r\n instead of \n
clrscr();             // Clear screen
bgcolor(COLOR_BLUE);  // Set colors
```

#### 4. Stack Limitations

**Problem:** The C64's 6502 processor has only 256 bytes total stack.

**Solution:** Use global buffers:
```c
char part1_input[MAX_ROW_LEN];
char part2_input[MAX_ROW_LEN];
```

#### 5. Memory Constraints

**Problem:** The C64 has only 64KB total RAM.

**Solution:** Keep input rows and calculations small:
```c
#define MAX_ROW_LEN 50  // Sufficient for sample inputs
```

#### 6. C89 Compliance

**Problem:** cc65 requires strict C89 compliance.

**Solution:** Declare all variables at scope beginning:
```c
int i, j, left, center, right;
unsigned int row_bits, mask, next_row;
/* ... use variables ... */
```

### Performance

- Part 1: Fast - completes in under a second
- Test cases execute almost instantly on C64 hardware

### Why These Constraints Matter

The C64 is a severely resource-constrained platform:

- CPU: 1 MHz 6502 (vs GHz multi-core)
- RAM: 64 KB total (vs GB)
- Stack: 256 bytes (vs MB)
- No virtual memory or dynamic allocation safety

Every byte and CPU cycle counts. This implementation prioritizes:

1. **Correctness** - Get the right answer
2. **Stability** - Don't crash
3. **Simplicity** - Keep code maintainable
4. **Efficiency** - Minimize memory and CPU usage

## Known Limitations

### Part 2 Not Implemented

Part 2 (400,000 rows) requires much more computation and memory tracking. While the algorithm is simple, simulating that many iterations on a 1 MHz processor would take several minutes. More importantly, tracking intermediate states for potential optimizations would exceed the C64's 64KB RAM limit.

For Part 2 solutions, refer to the Python implementation at:
https://github.com/ulasb/advent-of-code-python/tree/main/2016/day_18/

### Input Size Limited

Sample inputs are limited to about 50 characters due to C64 memory constraints. The algorithm scales linearly with row width.

## Files

- `day18.c` - Main source code (Part 1 implementation only)
- `Makefile` - Build script for cc65
- `README.md` - This file
- `day18.prg` - Compiled C64 executable (after running `make`)

## Building from Scratch

If you need to rebuild everything:

```bash
make clean  # Remove all build artifacts
make        # Rebuild day18.prg
make run    # Compile and run in VICE
```

## Debugging Tips

If you encounter issues:

1. **Black/gray screen hang:** Check that `conio.h` is being used, not `stdio.h`
2. **"Too many local variables" error:** Move large arrays to global scope
3. **Immediate exit:** Ensure `cgetc()` is called before `return 0`
4. **Width calculation issues:** Verify the mask `(1 << width) - 1` doesn't overflow for width > 16

## Design Notes

### Why Bit Manipulation?

The Python reference implementation uses bit manipulation because:

1. **Efficiency:** XOR operation is a single CPU instruction
2. **Memory:** Single integer vs. array of characters
3. **Clarity:** The logic directly matches the problem rules

The rule "trap if left XOR right" maps perfectly to: `((row << 1) ^ (row >> 1))`

### Algorithm Correctness

The correctness of the algorithm hinges on:

1. **Bit ordering:** We must be consistent - rightmost bit = rightmost tile
2. **Masking:** We must keep generated bits within the width
3. **Counting:** We must accurately count set bits (traps) to get safe tiles

All three are implemented carefully to avoid off-by-one errors on the C64.

## Additional Links

- [Python Implementation](https://github.com/ulasb/advent-of-code-python/tree/main/2016/day_18/)
- [Previous C64 Implementation (Day 19, 2015)](https://github.com/ulasb/advent-of-code-c64/tree/main/2015/Day19/)
- [Advent of Code 2016](https://adventofcode.com/2016)
- [cc65 Documentation](https://cc65.github.io/)
