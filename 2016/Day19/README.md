# Advent of Code 2016 - Day 19: An Elf House Divided

## Commodore 64 Port

This is a C implementation of the [Advent of Code 2016, Day 19](https://adventofcode.com/2016/day/19) challenge, optimized for the Commodore 64 using the `cc65` compiler.

### The Challenge

The problem is a variation of the Josephus problem.
- **Part 1**: Each elf takes all the presents from the elf to their left. The last elf remaining wins.
- **Part 2**: Each elf takes all the presents from the elf directly across the circle. The last elf remaining wins.

### Implementation Details

The solution uses $O(1)$ mathematical formulas to calculate the winner, which is essential for the C64 given the large input size (over 3 million elves) and limited memory/CPU.

- **Part 1 Formula**: $J(n) = 2(n - 2^{\lfloor \log_2 n \rfloor}) + 1$
- **Part 2 Formula**:
  - Let $p$ be the largest power of 3 such that $p \le n$.
  - If $n = p$, winner is $p$.
  - If $n \le 2p$, winner is $n - p$.
  - If $n > 2p$, winner is $2n - 3p$.

### C64 Optimizations

- **conio.h**: Uses fast console I/O for text output.
- **32-bit Arithmetic**: Uses `unsigned long` for calculations involving the input $N = 3,012,210$.
- **cc65 Compatibility**: Follows C89 standards and memory management best practices for the 6502 processor.

### Building and Running

To compile the program:
```bash
make
```

To run in the VICE emulator:
```bash
make run
```

### Requirements
- [cc65](https://cc65.github.io/) compiler
- [VICE](https://vice-emu.sourceforge.io/) emulator (for `make run`)
