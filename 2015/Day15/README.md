# Advent of Code 2015 - Day 15 (C64 Edition)

This is a Commodore 64 implementation of Advent of Code 2015 Day 15, written in C and compiled with cc65. You can get more information, a complete problem definition, and a larger input file on [Advent of Code](http://www.adventofcode.com).

## The Problem

Santa is trying to perfect his milk-dunking cookie recipe. He has various ingredients, each with specific properties (capacity, durability, flavor, texture, and calories). The goal is to find the optimal combination of ingredients to maximize the cookie's score.

### Part 1
Find the total score of the highest-scoring cookie you can make with the available ingredients. Each recipe must use exactly 100 teaspoons of ingredients.

The score is calculated by:
1. Multiply each ingredient's properties by the number of teaspoons used
2. Sum each property across all ingredients
3. Set any negative property totals to 0
4. Multiply the four property totals together (capacity × durability × flavor × texture)

### Part 2
Find the total score of the highest-scoring cookie you can make with exactly 500 calories.

## Running the Program

### Requirements
- cc65 compiler suite
- Commodore 64 emulator (like VICE) or real C64 hardware

### Compilation
```bash
make
```

This creates `day15.prg` which can be loaded and run on a Commodore 64.

### Testing
The program includes all the test cases specified for validation. All tests should pass with "(PASS)" indicators.

## Test Cases Included

The program validates against the exact test case provided in the problem description:

### Ingredients
- **Butterscotch**: capacity -1, durability -2, flavor 6, texture 3, calories 8
- **Cinnamon**: capacity 2, durability 3, flavor -2, texture -1, calories 3

### Part 1 Test
Recipe: 44 teaspoons Butterscotch + 56 teaspoons Cinnamon

**Property Calculations:**
- Capacity: 44×(-1) + 56×2 = 68
- Durability: 44×(-2) + 56×3 = 80
- Flavor: 44×6 + 56×(-2) = 152
- Texture: 44×3 + 56×(-1) = 76

**Expected Score:** 68 × 80 × 152 × 76 = **62,842,880**

### Part 2 Test
Recipe: 40 teaspoons Butterscotch + 60 teaspoons Cinnamon

**Calorie Calculation:**
- Total: 40×8 + 60×3 = 500 calories ✓

**Property Calculations:**
- Capacity: 40×(-1) + 60×2 = 80
- Durability: 40×(-2) + 60×3 = 100
- Flavor: 40×6 + 60×(-2) = 120
- Texture: 40×3 + 60×(-1) = 60

**Expected Score:** 80 × 100 × 120 × 60 = **57,600,000**

## Technical Notes

### Memory Usage
- **Ingredient storage**: Up to 4 ingredients with properties (~128 bytes)
- **Recursion stack**: Depends on number of ingredients (minimal for 2-4 ingredients)
- **Total memory usage**: <1KB for data structures, well within C64's ~40KB available program memory

### Data Types
- **Property values**: `int` for individual properties
- **Scores**: `long` (32-bit) to handle large score values (up to ~62 million)
- **Amounts**: `int` arrays for teaspoon counts

### Algorithm Implementation

The program implements a brute force search with recursive enumeration:

1. **Ingredient Setup**: Store each ingredient's properties in a structure
2. **Recursive Search**: Try all possible combinations of ingredient amounts that sum to 100 teaspoons
3. **Score Calculation**: For each combination, calculate the score according to the rules
4. **Optimization**: Track the best scores for both Part 1 (no constraint) and Part 2 (500 calories)

### Complexity Analysis

For `n` ingredients and `T` total teaspoons (100), the number of combinations is:
- **Formula**: C(T + n - 1, n - 1) using stars and bars
- **2 ingredients**: C(100 + 2 - 1, 2 - 1) = C(101, 1) = **101 combinations**
- **3 ingredients**: C(100 + 3 - 1, 3 - 1) = C(102, 2) = **5,151 combinations**
- **4 ingredients**: C(100 + 4 - 1, 4 - 1) = C(103, 3) = **176,851 combinations**

The C64 can handle up to 4 ingredients reasonably well, though execution time increases significantly.

### Performance Optimizations

The program implements several optimizations:

- **Single Pass**: Both Part 1 and Part 2 are solved in one recursive search
- **Early Exit**: Negative property values are clamped to 0 immediately
- **Long Arithmetic**: Uses 32-bit math only where necessary to avoid overflow
- **Stack Efficiency**: Minimizes recursion depth by handling the last ingredient as a base case

### Why This Approach?

Unlike the Python version which uses dictionary lookups and dynamic structures, this C64 implementation:
- Uses fixed-size arrays for predictable memory usage
- Implements manual recursion instead of Python's iterator approach
- Combines both parts into a single search pass for efficiency
- Uses simple integer arithmetic optimized for the 6502 processor

## Files
- `day15.c` - Main C source code with cookie optimization
- `Makefile` - Build configuration for cc65
