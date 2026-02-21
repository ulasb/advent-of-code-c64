# Advent of Code 2016 - Day 22: Grid Computing

This is a port of the Advent of Code 2016 Day 22 solution to C, specifically for the Commodore 64 using the `cc65` compiler.

## Problem Description
The task involves managing a grid of storage nodes.
- **Part 1**: Count "viable pairs" of nodes where one node's data can fit into another node's free space.
- **Part 2**: Find the minimum number of steps to move the goal data from the top-right corner to the top-left corner (0,0).

## Implementation Details
- The C64 version implements the logic using hardcoded grid parameters to stay within memory limits.
- Part 1 is solved by iterating over all node pairs.
- Part 2 uses a pathfinding approach to move the empty node to the goal data, then moves the goal data step-by-step.

## Building and Running
To compile the project, you need the `cc65` toolset. Run:
```bash
make
```
This will produce `day22.prg`, which can be run in a C64 emulator like VICE.

## Solution Result
- **Part 1**: 976
- **Part 2**: 209
