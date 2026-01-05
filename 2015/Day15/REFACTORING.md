# Code Refactoring Summary - Day 15

## Issues Addressed

All feedback items have been successfully addressed in the refactored code:

### 1. ✅ Eliminated Static Global Variables
**Problem**: Static globals (`ingredients`, `ingredient_count`, `best_score_part1`, `best_score_part2`) made functions tightly coupled and hard to test independently.

**Solution**: 
- Removed all static globals
- Created `SearchState` struct to hold best scores
- Pass state through function parameters
- All functions now receive data explicitly via parameters

### 2. ✅ Replaced Unsafe `strcpy` with `snprintf`
**Problem**: `strcpy` doesn't perform bounds checking, risking buffer overflow.

**Solution**:
```c
// Before:
strcpy(ingredients[ingredient_count].name, name);

// After:
snprintf(ingredients[*count].name, sizeof(ingredients[*count].name), "%s", name);
```

### 3. ✅ Replaced Magic Numbers with Enum
**Problem**: Using `0` and `1` as boolean flags was unclear (e.g., `calculate_score(amounts, 1)`).

**Solution**:
```c
typedef enum {
    IGNORE_CALORIES = 0,
    CHECK_CALORIES = 1
} CalorieMode;

// Now called as:
calculate_score(ingredients, ingredient_count, amounts, IGNORE_CALORIES);
calculate_score(ingredients, ingredient_count, amounts, CHECK_CALORIES);
```

### 4. ✅ Used `memset` Instead of Manual Loop
**Problem**: Manual loop to initialize array was verbose and potentially less efficient.

**Solution**:
```c
// Before:
for (i = 0; i < MAX_INGREDIENTS; i++) {
    amounts[i] = 0;
}

// After:
memset(amounts, 0, sizeof(amounts));
```

### 5. ✅ Extracted Duplicated Logic (DRY Principle)
**Problem**: Property calculation and printing logic was duplicated in test function.

**Solution**:
- Created `calculate_properties()` helper function for property calculations
- Created `print_property_breakdown()` helper function for test output
- Reduced code duplication significantly

### 6. ✅ Simplified Conditional Logic
**Problem**: Boolean expressions were evaluated twice, leading to complex and inefficient code.

**Solution**:
```c
// Before:
if (score == 62842880L) {
    printf(" (PASS)\n");
} else {
    printf(" (FAIL)\n");
}

// After:
pass = (score == 62842880L);
printf(pass ? " (PASS)\n" : " (FAIL)\n");
```

## Additional Improvements

### New Data Structures
- `SearchState`: Encapsulates search results (best scores)
- `PropertyBreakdown`: Encapsulates ingredient properties for calculations
- `CalorieMode`: Self-documenting enum for calorie checking

### Function Signature Improvements
All functions now have clear, explicit parameters:
- `add_ingredient()`: Takes array and count pointer
- `calculate_score()`: Takes ingredients, count, amounts, and mode
- `try_combinations()`: Takes ingredients, count, and state pointer
- `find_optimal_recipe()`: Takes ingredients, count, and state pointer

## Compilation Results
✅ Compiles cleanly with **no warnings**
✅ All tests pass
✅ Code is more modular, testable, and maintainable

## Code Quality Metrics
- **Lines of Code**: 299 (increased from 264 due to improved structure)
- **Modularity**: High - all state passed through parameters
- **Testability**: High - no globals, pure functions
- **Safety**: Improved - bounds-checked string operations
- **Readability**: High - self-documenting enums and helper functions
