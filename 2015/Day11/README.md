# Advent of Code 2015 - Day 11

Solution for Day 11 of Advent of Code 2015 on Commodore 64.

## Problem Description
Santa's previous password expired, and he needs help choosing a new one.

To find a new password, he rotates his old password string's characters. 
Passwords must follow these corporate policy requirements:
1. Passwords must include one increasing straight of at least three letters, like abc, bcd, cde, and so on, up to xyz. They cannot skip letters; abd doesn't count.
2. Passwords may not contain the letters i, o, or l, as these letters can be mistaken for other characters and are therefore confusing.
3. Passwords must contain at least two different, non-overlapping pairs of letters, like aa, bb, or cc.

## Build

Requires [cc65](https://cc65.github.io/).

```bash
make
```

## Run

Load `day11.prg` on a C64 emulator (like VICE) or hardware.

## Output
The program will output:
- Part 1: Next valid password after input.
- Part 2: Next valid password after Part 1 result.

## Performance Notes
- **Fast Skip**: The solution implements a smart skipping algorithm. When an invalid character ('i', 'o', 'l') is encountered, the password is immediately incremented at that position and the suffix reset to 'a's, skipping thousands of invalid iterations.
- **Direct Screen Access**: Uses `conio.h` for high-performance screen updates, providing real-time visual feedback without slowing down the calculation.
- **Optimized Validation**: Validity checks are streamlined to avoid redundant checks when skipping invalid characters.
- **C64 Specific Optimizations**:
    - **8-bit Arithmetic**: converted loop counters and flags to `unsigned char` to match the 8-bit 6502 CPU native word size, significantly reducing cycle counts compared to 16-bit `int`.
    - **Static Locals**: used `static` local variables to avoid the overhead of cc65's software stack for automatic variables.
    - **Global State**: Functions operate directly on global buffers to avoid parameter passing overhead.

> **Note**: Despite these optimizations, solving Part 2 involves checking a large number of potential passwords. On a real C64 (or accurate emulator) running at 1MHz, this process will take some time (potentially several minutes). The progress counter is updated periodically to show activity.
