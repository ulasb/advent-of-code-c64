# Advent of Code 2015 - Day 12: JSAbacusFramework.io

A Commodore 64 implementation of the Advent of Code 2015 Day 12 challenge, written in C for the `cc65` compiler.

## Problem Description

The challenge involves parsing JSON-like structures and summing up all these numbers:

- **Part 1:** Sum every number found in the document.
- **Part 2:** Sum all numbers, but ignore any object (and all its children) if any of its property values is the string `"red"`. This rule does not apply to arrays.

## Implementation Details

The C64 implementation focuses on memory efficiency and speed, avoiding the overhead of a full-blown JSON parser.

### Core Logic:
- **Part 1 Parser:** A simple linear scan of the input string, identifying and summing integers (including negatives).
- **Part 2 Parser:** A stack-based state machine that tracks JSON nesting levels (Objects and Arrays).
  - Uses a context stack (implemented as parallel arrays for `cc65` compatibility) to maintain local sums and flags for each level of nesting.
  - Correctly identifies property values in objects by tracking the `:` separator.
  - Handles nested structures by propagating sums from children to parents only if the "red" condition is not met.
  - Features robust string skipping (handling escaped characters like `\"`) and number parsing using `strtol`.

### Optimizations for C64:
- **Memory Management:** Uses a fixed-size stack for recursion levels, avoiding dynamic memory allocation (`malloc`/`free`). Includes stack overflow protection for safety.
- **Data Types:** Uses `long` (32-bit) for sums to prevent overflow, as AOC inputs can result in values exceeding 16-bit limits.
- **Efficiency:** Single-pass parsing for Part 1 and stack-based single-pass for Part 2 using `strtol` for robust number conversion.

## Compiling and Running

### Prerequisites:
- [cc65](https://cc65.github.io/) compiler suite installed.
- (Optional) [VICE](https://vice-emu.sourceforge.io/) emulator for running the `.prg` file.

### Build:
```bash
make
```

### Run:
```bash
# To run with VICE
x64 day12.prg
```

## Validation Tests

The program includes built-in tests based on the problem examples:

### Part 1:
- `[1,2,3]` -> 6
- `{"a":2,"b":4}` -> 6
- `[[[3]]]` -> 3
- `{"a":{"b":4},"c":-1}` -> 3
- `{"a":[-1,1]}` -> 0
- `[-1,{"a":1}]` -> 0
- `[]` -> 0
- `{}` -> 0

### Part 2:
- `[1,2,3]` -> 6
- `[1,{"c":"red","b":2},3]` -> 4
- `{"d":"red","e":[1,2,3,4],"f":5}` -> 0
- `[1,"red",5]` -> 6
