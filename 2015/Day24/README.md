# Advent of Code 2015 - Day 24 (C64 Edition)

## It Hangs in the Balance
This project implements a solution for the [Advent of Code 2015 Day 24](https://adventofcode.com/2015/day/24) puzzle, optimized for the Commodore 64.

The goal is to partition a list of package weights into 3 (Part 1) or 4 (Part 2) groups of equal total weight. We must minimize the number of packages in the first group, and then minimize the "Quantum Entanglement" (the product of the weights in that group).

## Implementation Details
The C64's 1 MHz processor and memory constraints require a highly optimized search algorithm.

### Algorithm
- **Recursive Combination Search**: The program iterates through possible group sizes `r` starting from 1.
- **Pruning**: To avoid exploring the massive search space (millions of combinations), the algorithm uses:
  - **Sorted Search**: Weights are searched in descending order.
  - **Upper Bound Pruning**: If the current weight multiplied by the remaining number of items to pick is less than the remaining target, the branch is skipped.
  - **QE Pruning**: If a combination's partial product already exceeds the best "Quantum Entanglement" found so far, it is pruned early.
- **64-bit Math**: Since Quantum Entanglement results easily exceed 4 billion (32-bit `unsigned long`), a custom 64-bit multiplication and decimal formatting library was implemented to handle the large products.

## Running the Program
### Requirements
- [cc65](https://cc65.github.io/) compiler.
- A C64 emulator like [VICE](https://vice-emu.sourceforge.io/) (x64sc) or a real Commodore 64.

### Compilation
Run the following command in the project directory:
```bash
make
```
This will generate `day24.prg`.

### Execution
Load the `.prg` file into your emulator or onto your C64.
```bash
x64sc -autostart day24.prg
```

## Test Cases Included
The program runs through two sets of data:
1. **Example Data**: The 10 packages (1-5, 7-11) provided in the puzzle description.
   - Part 1 Target: 20
   - Part 1 Min Size: 2
   - Part 1 Min QE: 99
2. **Real Data Simulation**: A typical Advent of Code input consisting of ~30 packages (based on primes and common input patterns).

## Technical Notes
- **Memory**: The program uses minimal memory, keeping the weight list and bitwise structures in low RAM.
- **64-bit Decimal Conversion**: Implementing `uint64` to string was necessary for the C64, as the standard `printf` does not support 64-bit integers.
- **Performance**: On a real C64 (or 100% speed emulator), the search for Part 1 with 30 items takes approximately 10-30 seconds depending on the specific weights, thanks to aggressive pruning.

## Files
- `day24.c`: The main source code.
- `Makefile`: Build instructions for `cc65`.
- `README.md`: This documentation.

## License
This code is licensed under the Mozilla Public License 2.0.
