# Advent of Code 2015 - Day 22: Wizard Simulator 20XX

This project implements a solution for Advent of Code 2015, Day 22, in C for the Commodore 64.

## Problem Description

The problem is a turn-based combat simulation between a Wizard (player) and a Boss. The player can cast various spells consuming mana. Some spells have immediate effects, while others are "effects" that last for several turns. The goal is to find the minimum amount of mana spent to defeat the boss.

### Part 1
Standard combat. Find the minimum mana to win.

### Part 2
Hard Mode: The player loses 1 HP at the start of each player turn. Find the minimum mana to win.

## Implementation Details

The solution uses a recursive depth-first search (DFS) with pruning (branch and bound). Since the Commodore 64 has limited memory and stack space, the search is optimized by:
- Using a compact `GameState` structure passed by pointer.
- **UI Design**: Uses `conio.h` for a structured display. Results are placed at fixed coordinates to prevent flickering and overlapping, and a dedicated progress tracker shows the number of states explored in real-time.

### Technical Challenges on C64
- **Display Stability**: To avoid overlapping text from recursive updates, the program uses fixed screen positions (`gotoxy`) and clear-line commands (`cclear`). A dedicated status area at the bottom provides live feedback on the search progress without interfering with the results.
- **Code Refactoring**: Redundant logic for applying spell effects (shield, poison, recharge) has been extracted into a unified `apply_effects` function, improving maintainability and ensuring consistent rule application for both player and boss turns.
- **Memory/Stack**: DFS can be risky on 6502 due to the small hardware stack. However, the depth of the combat tree is relatively shallow (typically < 20 turns), making it feasible within cc65's soft stack.
- **Performance**: The search space can be large. Pruning and simple memoization are used to ensure the program finishes in a reasonable time.
- **Build System**: The Makefile includes `.PHONY` targets for `all` and `clean` to ensure reliable builds regardless of the local file state.

## Requirements

- [cc65](https://cc65.github.io/) compiler
- A C64 emulator like [VICE](https://vice-emu.sourceforge.io/) or original hardware.

## Building

To compile the project and generate `day22.prg`:

```bash
make
```

## Running

Load `day22.prg` into your C64 emulator or onto physical media.

```bash
x64sc day22.prg
```

## Testing

The program uses the following test values (no input file is used):
- **Boss HP**: 51
- **Boss Damage**: 9
- **Player HP**: 50
- **Player Mana**: 500
