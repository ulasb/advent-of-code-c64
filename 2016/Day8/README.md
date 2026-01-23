# Advent of Code 2016 - Day 8 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2016 Day 8, written in C and compiled with cc65.

## The Problem

**Day 8: Two-Factor Authentication**

You come across a magnetic card reader and a small screen. The screen is 50 pixels wide and 6 pixels tall. All pixels are initially off.
Commands:
- **rect AxB:** turns on all of the pixels in a rectangle at the top-left of the screen which is A wide and B tall.
- **rotate row y=A by B:** shifts all of the pixels in row A right by B pixels.
- **rotate column x=A by B:** shifts all of the pixels in column A down by B pixels.

**Part 1:** After all commands, how many pixels are lit? ✅ **Implemented**
**Part 2:** What is the message displayed on the screen? ✅ **Implemented** (Visual)

## Running the Program

### Requirements
- [cc65](https://cc65.github.io/) compiler suite
- [VICE](https://vice-emu.sourceforge.io/) or another C64 emulator (like `x64`)

### Compilation
To compile the program, run:
```bash
make
```
This produces `day8.prg`.

### Testing
To run it in VICE:
```bash
make run
```
Or load `day8.prg` manually into your emulator.

The program will run automated tests (based on the problem description example) and display the results on screen.

## Test Cases Included

We use hardcoded test cases derived from the problem description to verify the solution logic.

### Example Sequence
- `rect 3x2`
- `rotate column x=1 by 1`
- `rotate row y=0 by 4`
- `rotate column x=1 by 1`
**Expected:** 6 pixels lit, and a specific pattern displayed. ✅

## Technical Notes

### Algorithm

**Grid Representation:**
- A simple 2D array `char grid[6][50]` stores the state of each pixel.
- `.` represents an off pixel, `#` represents an on pixel.

**Transformations:**
- `rect`: Simple nested loops.
- `rotate row`: Buffer the row and shift with modulo indexing.
- `rotate col`: Buffer the column and shift with modulo indexing.

### C64-Specific Challenges and Solutions

#### 1. Stack and Memory
- The grid is stored in the global BSS segment to avoid stack overflow. 
- Total grid size is 300 bytes, well within the C64's capacity.

#### 2. Screen Output
- Uses `conio.h` for reliable screen output.
- `cprintf` and `cputc` are used for direct character rendering.
- `bgcolor`, `bordercolor`, and `textcolor` are used to set a "terminal-like" look.

#### 3. 40-Column Screen Handling
**Problem:** The C64 screen is 40 columns wide, but the puzzle grid is 50 columns. Printing a full row causes automatic wrapping, making the Part 2 message unreadable.

**Solution:** The logic splits the 50x6 grid into two 25x6 blocks (Left Half and Right Half) printed sequentially. This prevents wrapping and maintains the visual integrity of the 5x6 pixel characters used for the message.

## Files
- `day8.c` - Main source code
- `Makefile` - Build script
- `README.md` - This file

---
Implementation by Antigravity (Google Deepmind) for ulasb.
