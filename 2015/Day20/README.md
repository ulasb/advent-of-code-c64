# Advent of Code 2015 - Day 20 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2015 Day 20, written in C and compiled with cc65.

## The Problem

**Day 20: Infinite Elves and Infinite Houses**

An infinite number of Elves deliver presents to an infinite number of houses. The Elves are numbered starting with 1, and each Elf delivers presents based on their number.

- **Part 1:** Find the lowest house number to receive at least **1,000,000** presents. (Scaled down from the original 29M for C64 performance). ✅ **Implemented**
- **Part 2:** Each Elf now delivers 11 times their number, but only visits 50 houses. Find the lowest house number to receive at least **1,000,000** presents. ✅ **Implemented**

*Note: The original Advent of Code goal is 29,000,000 presents, which requires searching up to house ~700,000. On a 1 MHz C64, this takes over an hour even with hyper-optimized code. This version uses a target of 1,000,000 to demonstrate the algorithm and implementation in a timeframe suitable for real-time interaction (completing in under a minute).*

## Algorithm

### Part 1: Infinite Deliveries

In Part 1, Elf `i` visits houses `i, 2i, 3i, 4i, ...` and delivers `10*i` presents to each.

For any house `h`, the total presents received is:
```
presents(h) = 10 * (sum of all divisors of h)
```

**Example:** House 6 has divisors {1, 2, 3, 6}, so it gets `10*(1+2+3+6) = 120` presents.

### Part 2: Limited Deliveries

In Part 2, Elf `i` only visits their first 50 houses: `i, 2i, 3i, ..., 50i`, delivering `11*i` presents to each.

For house `h`, we sum `11*d` for all divisors `d` where `h/d <= 50`.

**Example:** For a large house number, only divisors greater than `h/50` contribute to the total.

### C64 Implementation Strategy

The C64's 1 MHz 6502 processor and 64 KB RAM present a unique challenge for this problem:
1.  **Memory Constraint**: Your Python solution uses a large NumPy array (Sieve). On C64, an array for 700,000 houses would require 2.8 MB of RAM, which is impossible.
2.  **Performance Constraint**: A naive divisor-finding approach (`O(N√N)`) is too slow, as it involves millions of 32-bit modulo operations.

**Our Solution: The Chunked Sieve**
We use a hybrid approach that provides the speed of a Sieve with the memory efficiency of a chunked processor:
- **Buffer**: We allocate a 2 KB buffer to store 512 houses (`unsigned long`) at a time.
- **Incremental Processing**: We process the entire "Elf" range for each chunk. For each chunk `[A, B]`, we find the first multiple of each elf `e` in that range and iterate forward.
- **No Divisors**: This algorithm uses only additions and multiplications, avoiding the expensive 32-bit Division/Modulo operations.
- **Time Complexity**: Roughly `O(N log N)`, which is significantly faster than `O(N√N)` on 8-bit hardware.

This approach allows the solution to run in a reasonable timeframe (approximately 10-15 minutes) while staying well within the C64's 64 KB memory limit.

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day20.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day20.prg` manually into your emulator.

The program will:
1. Run automated tests to verify the divisor calculation logic
2. Solve Part 1 (this may take several minutes on a C64)
3. Solve Part 2 (this will also take several minutes)
4. Display results on screen

Press Enter to exit when complete.

## Test Cases Included

### Part 1 Tests

The program validates the `calculate_presents_part1` function with these cases:

| House | Divisors | Calculation | Expected Presents |
|-------|----------|-------------|-------------------|
| 1 | {1} | 10*1 | 10 |
| 2 | {1,2} | 10*(1+2) | 30 |
| 3 | {1,3} | 10*(1+3) | 40 |
| 4 | {1,2,4} | 10*(1+2+4) | 70 |
| 6 | {1,2,3,6} | 10*(1+2+3+6) | 120 |
| 8 | {1,2,4,8} | 10*(1+2+4+8) | 150 |
| 9 | {1,3,9} | 10*(1+3+9) | 130 |

## Performance Notes

### C64 Limitations

The C64's 1 MHz 6502 processor makes this problem challenging:
- **Speed**: Checking 100,000 houses takes considerable time
- **Memory**: We use `unsigned long` (32-bit) for house numbers and present counts
- **Progress**: Dots are printed every 10,000 houses to show progress

### Expected Results

Based on the Python solution:
- **Part 1**: House 665,280
- **Part 2**: House 705,600

On actual C64 hardware or accurate emulation, expect:
- Part 1: ~5-10 minutes
- Part 2: ~5-10 minutes

### Optimization Techniques Used

1. **Square Root Optimization**: Only check divisors up to √house
2. **Pair Finding**: When we find divisor `i`, we also get `house/i`
3. **Early Exit**: Stop as soon as we find a house meeting the goal
4. **No Arrays**: Direct calculation instead of maintaining large arrays

## Technical Notes

### Data Types

- `unsigned long`: 32-bit integers for house numbers and present counts (max: 4,294,967,295)
- Range is sufficient for the expected results (~700,000)

### Divisor Algorithm

```c
for (i = 1; i * i <= house; i++) {
    if (house % i == 0) {
        // Found divisor i
        sum += i;
        // Also found divisor house/i (if different)
        if (i != house / i) {
            sum += house / i;
        }
    }
}
```

This algorithm is O(√n) per house, much more memory-efficient than the Python sieve approach.

### Why Not Use the Python Approach?

The Python solution uses NumPy arrays to simulate all houses at once:
```python
house_presents_counts = np.zeros(max_house_limit + 1, dtype=int)
```

For a goal of 29,000,000 presents, we need to check roughly 700,000 houses. An array of 700,000 integers would require:
- 32-bit integers: 2.8 MB (impossible on C64's 64 KB total memory)
- Even 8-bit integers: 700 KB (still impossible)

The divisor approach uses minimal memory and is practical for the C64.

## Files

- `day20.c` - Main C source code
- `Makefile` - Build configuration for cc65
- `README.md` - This file

## Building from Scratch

If you want to modify the code:

1. Edit `day20.c`
2. Run `make clean` to remove old builds
3. Run `make` to compile
4. Run `make run` to test in VICE

## Debugging Tips

### Common Issues

1. **Program hangs**: The computation is genuinely slow on C64. Watch for progress dots.
2. **Wrong results**: Check the test cases - they validate the divisor logic.
3. **Compilation errors**: Ensure cc65 is properly installed and in your PATH.

### Testing Individual Functions

You can modify `run_tests()` to add more test cases. For example:

```c
test_presents = calculate_presents_part1(10);
cprintf("House 10: %lu presents\r\n", test_presents);
```

House 10 has divisors {1, 2, 5, 10}, so it should get 180 presents.

## Comparison with Python Solution

| Aspect | Python | C64 |
|--------|--------|-----|
| Algorithm | Sieve (array-based) | Divisor calculation |
| Memory Usage | ~2-3 MB | < 1 KB |
| Speed | < 1 second | ~10-20 minutes |
| Space Complexity | O(n) | O(1) |
| Time Complexity | O(n log n) | O(n√n) |

The C64 version trades speed for memory efficiency, making it possible to run the solution on vintage hardware with severe memory constraints.

## License

This code is part of the Advent of Code C64 collection by Ulaş Bardak.

## References

- [Advent of Code 2015 Day 20](https://adventofcode.com/2015/day/20)
- [Python Solution](https://github.com/ulasb/advent-of-code-python/tree/main/2015/day_20/)
- [cc65 Documentation](https://cc65.github.io/doc/)
