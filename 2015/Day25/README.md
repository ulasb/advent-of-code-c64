# Advent of Code 2015 - Day 25 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2015 Day 25, written in C and compiled with cc65.

## The Problem

**Day 25: Let It Snow**

Santa needs to access the weather machine, but it requires a specific code from a grid of codes that follow a mathematical sequence. The codes are arranged in a diagonal pattern and each code is generated using a specific formula.

- **Part 1:** Find the code at row 2947, column 3029 in the infinite grid ✅ **Implemented**

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day25.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day25.prg` manually into your emulator.

The program will run automated tests and display results on screen. Press Enter to exit when complete.

## Test Cases Included

Since the C64 has limited memory and I/O capabilities, we use hardcoded test cases derived from the problem description to verify the solution logic.

### Verification Tests
The program includes all 36 test cases from the provided 6×6 grid of example codes to ensure correctness:

| Row/Col | 1      | 2      | 3      | 4      | 5      | 6      |
|---------|--------|--------|--------|--------|--------|--------|
| **1**   | 20151125 | 18749137 | 17289845 | 30943339 | 10071777 | 33511524 |
| **2**   | 31916031 | 21629792 | 16929656 | 7726640 | 15514188 | 4041754 |
| **3**   | 16080970 | 8057251 | 1601130 | 7981243 | 11661866 | 16474243 |
| **4**   | 24592653 | 32451966 | 21345942 | 9380097 | 10600672 | 31527494 |
| **5**   | 77061 | 17552253 | 28094349 | 6899651 | 9250759 | 31663883 |
| **6**   | 33071741 | 6796745 | 25397450 | 24659492 | 1534922 | 27995004 |

All test cases must pass for the solution to be considered correct.

## Technical Notes - Implementation

### Algorithm

**Diagonal Sequence Calculation:**
The grid positions follow a diagonal pattern where each diagonal has increasing numbers. The index for position (row, col) is calculated as:
```
index = ((row + col - 2) * (row + col - 1)) / 2 + col
```

**Code Generation:**
Each code is generated using modular exponentiation with overflow-safe arithmetic:
```
code = safe_mod_mult(20151125, mod_pow(252533, index - 1, 33554393), 33554393)
```

### C64-Specific Challenges and Solutions

#### 1. Integer Size Requirements
**Problem:** The modulus (33554393) exceeds 16-bit integers, and intermediate calculations in modular exponentiation require 32-bit arithmetic.

**Solution:** Use `long` (32-bit) integers throughout:
```c
#define INITIAL_CODE 20151125L
#define MULTIPLIER 252533L
#define MODULUS 33554393L
```

#### 2. Modular Exponentiation Implementation
**Problem:** C89/cc65 has no built-in modular exponentiation, and 32-bit integer overflow causes incorrect results during large multiplications.

**Solution:** Implement binary exponentiation with overflow-safe modular multiplication:
```c
long safe_mod_mult(long a, long b, long modulus) {
  /* Safe modular multiplication to avoid overflow */
  long result = 0;
  a = a % modulus;
  b = b % modulus;

  while (b > 0) {
    if (b % 2 == 1) {
      result = (result + a) % modulus;
    }
    a = (a * 2) % modulus;
    b = b / 2;
  }

  return result;
}

long mod_pow(long base, long exponent, long modulus) {
  long result = 1;
  base = base % modulus;

  while (exponent > 0) {
    if (exponent % 2 == 1) {
      result = safe_mod_mult(result, base, modulus);
    }
    base = safe_mod_mult(base, base, modulus);
    exponent = exponent / 2;
  }

  return result;
}
```

This prevents overflow during intermediate calculations by using additive decomposition instead of direct multiplication.

#### 3. C89 Compliance and Variable Declaration
**Problem:** cc65 requires strict C89 compliance - all variables must be declared at the beginning of their scope.

**Solution:** Declare all variables at function start:
```c
long get_code(int row, int col) {
  long index = get_index(row, col);
  long exponent = index - 1;
  // ... rest of function
}
```

#### 4. Output Reliability and Screen Management
**Problem:** Standard `stdio.h` functions were unreliable on C64, and the 40×25 character screen fills quickly with test output.

**Solution:** Use `conio.h` for direct screen access with careful output management:
```c
#include <conio.h>
cprintf("TEXT\r\n");  // Use \r\n instead of \n
clrscr();             // Clear screen
bgcolor(COLOR_BLUE);  // Set colors
gotoxy(0, 20);        // Position cursor for final output
```

Benefits:
- No buffering issues
- Direct character-mode output
- Reliable on C64 hardware
- Limited test output prevents screen overflow (8 key tests instead of 36)
- Clear pass/fail indication with compact messages

#### 5. Program Exit Behavior
**Problem:** When `main()` returns, the C64 immediately returns to BASIC, clearing the screen.

**Solution:** Wait for user input before exiting:
```c
cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
cgetc(); // Wait for any key press
```

### Performance

- **Index Calculation:** O(1) - simple arithmetic
- **Modular Exponentiation:** O(log n) where n is the index (fast even for large indices)
- **Overall:** Completes almost instantly on C64 hardware

### Why This Problem Fits the C64 Perfectly

Day 25 is ideally suited for C64 implementation because:
- **Pure Mathematics:** No string manipulation or complex data structures
- **Deterministic Algorithm:** No randomness or complex state management
- **Fixed Memory Usage:** Only a few variables needed
- **Fast Execution:** Modular exponentiation is computationally inexpensive

Compare this to Day 19's string replacement algorithm, which required dynamic buffers and complex state tracking.

## Files

- `day25.c` - Main source code with complete implementation
- `Makefile` - Build script for cc65
- `README.md` - This file
- `day25.prg` - Compiled C64 executable (after running `make`)

## Building from Scratch

If you need to rebuild everything:
```bash
make clean  # Remove all build artifacts
make        # Rebuild day25.prg
make run    # Compile and run in VICE
```

## Debugging Tips

If you encounter issues:
1. **Black/gray screen hang:** Check that `conio.h` is being used, not `stdio.h`
2. **Integer overflow:** Ensure all constants use `L` suffix for 32-bit literals
3. **Immediate exit:** Ensure `cgetc()` is called before `return 0`
4. **Wrong results:** Verify modular arithmetic - C's `%` operator works with negative numbers differently than expected

---

## Comparison with Python Implementation

The Python version uses built-in `pow(base, exponent, modulus)` for modular exponentiation, which is equivalent to our `mod_pow` function. The C64 version demonstrates that even complex mathematical algorithms can run efficiently on severely resource-constrained hardware.

For the Python implementation, see: https://github.com/ulasb/advent-of-code-python/tree/main/2015/day_25/
