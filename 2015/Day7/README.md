# Advent of Code 2015 - Day 7 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2015 Day 7, written in C and compiled with cc65. You can get more information, a complete problem definition, and a larger input file on [Advent of Code](http://www.adventofcode.com).

## The Problem

This year, Santa brought little Bobby Tables a set of wires and bitwise logic gates! Unfortunately, little Bobby is a little under the recommended age range, and he needs help assembling the circuit.

### Part 1
The problem involves simulating a digital circuit with wires that can carry 16-bit signals (0-65535). Each wire can:

- Carry a constant signal value
- Reference another wire's signal
- Apply bitwise operations: AND, OR, NOT, left shift (LSHIFT), right shift (RSHIFT)

The goal is to determine the signal value on wire "a" after resolving all dependencies in the circuit.

## Running the Program

### Requirements
- cc65 compiler suite
- Commodore 64 emulator (like VICE) or real C64 hardware

### Compilation
```bash
make
```

This creates `day7.prg` which can be loaded and run on a Commodore 64.

### Testing
The program includes all the test cases specified for validation. The circuit simulation uses memoization to efficiently resolve wire dependencies and prevent infinite loops.

All tests should pass with "(PASS)" indicators.

## Test Cases Included

The program validates against the exact test circuit provided in the problem description:

### Circuit Definition
- `123 -> x`
- `456 -> y`
- `x AND y -> d`
- `x OR y -> e`
- `x LSHIFT 2 -> f`
- `y RSHIFT 2 -> g`
- `NOT x -> h`
- `NOT y -> i`

### Expected Results
- `d`: 72 (123 AND 456)
- `e`: 507 (123 OR 456)
- `f`: 492 (123 << 2)
- `g`: 114 (456 >> 2)
- `h`: 65412 (~123 & 0xFFFF)
- `i`: 65079 (~456 & 0xFFFF)
- `x`: 123 (direct assignment)
- `y`: 456 (direct assignment)

## Technical Notes

### Memory Usage
- **Wire storage**: 16 wires maximum with names and expressions (~1KB total)
- **Memoization cache**: Each wire caches its resolved 16-bit value
- **Total memory usage**: ~2KB for circuit storage, well within C64's ~40KB available program memory

### Data Types
- **Wire values**: `unsigned int` (16-bit) to properly handle bitwise operations and overflow
- **Wire names**: Fixed-size character arrays for C64 compatibility
- **Expressions**: String storage for parsing operations

### Algorithm Implementation

The program implements a recursive circuit resolver with memoization:

1. **Parsing**: Each wire definition is parsed into name and expression components
2. **Resolution**: Wires are resolved recursively, with each wire's value cached after first computation
3. **Operations**: All bitwise operations (AND, OR, NOT, LSHIFT, RSHIFT) are implemented with proper 16-bit masking
4. **Dependencies**: Circular dependencies are prevented through memoization

### Why Not Larger Circuits?
While the algorithm can handle much larger circuits, the C64's memory constraints limit the maximum number of wires. Real Advent of Code inputs typically have hundreds of wires, which would exceed C64 memory limits.

### Performance Optimizations

The program implements several C64-specific optimizations:

- **Memoization**: Each wire value is computed only once and cached
- **Safe tokenization**: Expression parsing uses a temporary buffer to avoid modifying original strings
- **Simple parsing**: Custom string tokenization instead of `strtok()` for C64 compatibility
- **Direct operations**: Bitwise operations use efficient C operators
- **Fixed-size arrays**: Avoids dynamic memory allocation for C64 reliability

### Safe Tokenization Design

The `resolve_wire()` function initially used in-place string modification for tokenization to save memory. However, this created a destructive side effect where original expression strings were corrupted with null terminators after resolution. This fix uses a temporary buffer (`temp_expr`) to safely tokenize expressions without modifying the original data, preventing potential bugs in debugging or display functions.

## Files
- `day7.c` - Main C source code with circuit simulation
- `Makefile` - Build configuration for cc65
