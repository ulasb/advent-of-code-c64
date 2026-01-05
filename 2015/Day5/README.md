# Advent of Code 2015 - Day 5 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2015 Day 5, written in C and compiled with cc65. You can get more information, a complete problem definition, and a larger input file on [Advent of Code](http://www.adventofcode.com).

## The Problem

Santa needs help figuring out which strings in his text file are "naughty" or "nice" according to specific criteria.

### Part 1
A nice string is one with all of the following properties:
- It contains at least three vowels (aeiou only), like aei, xazegov, or aeiouaeiouaeiou.
- It contains at least one letter that appears twice in a row, like xx, abcdde (dd), or aabbccdd (aa, bb, cc, or dd).
- It does not contain the strings ab, cd, pq, or xy, even if they are part of one of the other requirements.

### Part 2
Now, a nice string is one with all of the following properties:
- It contains a pair of any two letters that appears at least twice in the string without overlapping, like xyxy (xy) or aabcdefgaa (aa), but not like aaa (aa, but it overlaps).
- It contains at least one letter which repeats with exactly one letter between them, like xyx, abcdefeghi (efe), or even aaa.

## Running the Program

### Requirements
- cc65 compiler suite
- Commodore 64 emulator (like VICE) or real C64 hardware

### Compilation
```bash
make
```

This creates `day5.prg` which can be loaded and run on a Commodore 64.

### Testing
The program includes all the test cases from the problem description:
- Part 1: 5 test cases covering the three rules for nice strings
- Part 2: 4 test cases covering the two new rules for nice strings

All tests should pass with "(PASS)" indicators.

## Test Cases Included

### Part 1 Tests
- `ugknbfddgicrmopn` is nice because it has at least three vowels (u...i...o...), a double letter (...dd...), and none of the disallowed substrings.
- `aaa` is nice because it has at least three vowels and a double letter, even though the letters used by different rules overlap.
- `jchzalrnumimnmhp` is naughty because it has no double letter.
- `haegwjzuvuyypxyu` is naughty because it contains the string xy.
- `dvszwmarrgswjxmb` is naughty because it contains only one vowel.

### Part 2 Tests
- `qjhvhtzxzqqjkmpb` is nice because it has a pair that appears twice (qj) and a letter that repeats with exactly one letter between them (zxz).
- `xxyxx` is nice because it has a pair that appears twice and a letter that repeats with one between, even though the letters used by each rule overlap.
- `uurcxstgmygtbstg` is naughty because it has a pair (tg) but no repeat with a single letter between them.
- `ieodomkazucvgmuy` is naughty because it has a repeating letter with one between (odo), but no pair that appears twice.

## Files
- `day5.c` - Main C source code
- `Makefile` - Build configuration
- `day5.prg` - Compiled Commodore 64 program
