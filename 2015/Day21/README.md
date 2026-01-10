# Advent of Code 2015 - Day 21: RPG Simulator 20XX (C64)

This repository contains the C implementation of the Advent of Code 2015 Day 21 challenge, compiled for the Commodore 64 using the `cc65` toolchain.

## Problem Description

The challenge involves finding the most and least efficient equipment sets to defeat a boss in a turn-based RPG. The player has 100 hit points and can equip items from three categories:
- **Weapons**: Must equip exactly one.
- **Armor**: Can equip zero or one.
- **Rings**: Can equip zero, one, or two (each must be unique).

Combat is deterministic: the attacker deals damage equal to `attacker_damage - defender_armor`, with a minimum of 1 damage. The player attacks first.

## Implementation Details

The solution is implemented in C and optimized for the C64's 6502 processor.

- **Item Tables**: Fixed arrays for Weapons, Armor, and Rings.
- **Nested Loops**: Iterates through all valid combinations of equipment (5 weapons × 6 armor options × 28 ring combinations = 840 total scenarios).
- **Optimized Simulation**: Instead of a turn-by-turn loop, the program calculates the number of rounds to win/lose using integer division (`(hp + dmg - 1) / dmg`). This significantly improves performance on 8-bit hardware by eliminating unnecessary looping and branching.
- **Memory Optimization**: Uses `unsigned char` and `unsigned int` where appropriate to minimize 16-bit overhead on the 8-bit CPU.
- **Build System**: A standard `Makefile` with `.PHONY` targets for robust project management.

## Boss Stats

Since there is no input file provided for this day, the following stats were used (typical for this challenge):
- **Hit Points**: 104
- **Damage**: 8
- **Armor**: 1

## Building and Running

### Prerequisites

- [cc65](https://cc65.github.io/) compiler
- A C64 emulator like [VICE](http://vice-emu.sourceforge.net/) (optional, for running the `.prg`)

### Compilation

To compile the project, run:

```bash
make
```

This will generate `day21.prg`, which can be loaded into a C64 or emulator.

## Results

The program outputs:
1. **Part 1**: The minimum amount of gold needed to win the fight.
2. **Part 2**: The maximum amount of gold that can be spent and still lose the fight.
