# Advent of Code 2015 - Day 14 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2015 Day 14, written in C and compiled with cc65. You can get more information, a complete problem definition, and a larger input file on [Advent of Code](http://www.adventofcode.com).

## The Problem

This year, Santa has some reindeer that can fly at high speeds, but they also need to rest between flights. Each reindeer has different flight characteristics - speed, flight time, and rest time.

### Part 1
The goal is to find out which reindeer travels the farthest distance after exactly 2503 seconds of racing.

### Part 2
In Part 2, points are awarded each second to the reindeer currently in the lead. The reindeer with the most points after 2503 seconds wins.

## Running the Program

### Requirements
- cc65 compiler suite
- Commodore 64 emulator (like VICE) or real C64 hardware

### Compilation
```bash
make
```

This creates `day14.prg` which can be loaded and run on a Commodore 64.

### Testing
The program includes the test cases specified in the problem description for validation. The solver simulates each second of the race for all reindeer.

All tests should pass with "(PASS)" indicators.

## Test Cases Included

The program validates against the following test data:

### Part 1 Test Data
- Comet: 14 km/s for 10 seconds, rest 127 seconds
- Dancer: 16 km/s for 11 seconds, rest 162 seconds
- After 1000 seconds: Comet travels **1120 km**, Dancer travels **1056 km**

### Part 2 Test Data
- Same reindeer as Part 1
- After 1000 seconds: Dancer earns **689 points**, Comet earns **312 points**

## Technical Notes

### Memory Usage
- **Reindeer Array**: 10 reindeer structures (~300 bytes total)
- **Reindeer Structure**: Name (16 chars), speed, fly_time, rest_time, distance, points, flying state, time counter (30 bytes each)

### Data Types
- **Distances and Points**: `int` (16-bit signed) - sufficient for 2503 seconds at reasonable speeds
- **Names**: Fixed-size character arrays (16 bytes)
- **Time counters**: `int` (16-bit signed)
- **Reindeer State**: `ReindeerState` enum (maps to int) - REINDEER_FLYING or REINDEER_RESTING

### Algorithm Implementation

The program implements a second-by-second reindeer race simulation:

1. **State Tracking**: Each reindeer tracks its current distance, points, flying/resting state (using ReindeerState enum), and time in current state.
2. **Second-by-Second Updates**: For each second, update each reindeer's position and state based on their flight characteristics.
3. **Point Awards**: In Part 2, find the current leader(s) each second and award points.
4. **Race Simulation**: Run the full 2503-second simulation for both distance and points calculations.
5. **Modular Design**: Functions accept reindeer arrays and counts as parameters, making them testable and reusable.

### Performance Optimizations

The program implements several C64-specific optimizations:

- **Direct State Updates**: Simple integer arithmetic for position and state updates.
- **Minimal Memory Footprint**: Fixed-size arrays with no dynamic allocation.
- **Efficient Comparisons**: Single pass through reindeer array for leader detection each second.
- **Type-Safe State Management**: ReindeerState enum provides clear, self-documenting state checks.

## Files
- `day14.c` - Main C source code with reindeer race simulation
- `Makefile` - Build configuration for cc65
