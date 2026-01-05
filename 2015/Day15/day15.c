/*
 * Advent of Code 2015 - Day 15 (C64 Edition)
 * Science for Hungry People - Cookie Recipe Optimizer
 *
 * This program finds the optimal cookie recipe that maximizes score.
 * Part 1: Find the highest score possible with any recipe (100 teaspoons
 * total). Part 2: Find the highest score with exactly 500 calories.
 */

#include <stdio.h>
#include <string.h>

#define MAX_INGREDIENTS 4   /* Maximum number of ingredients */
#define TOTAL_TEASPOONS 100 /* Total teaspoons to use */
#define CALORIE_TARGET 500  /* Target calories for Part 2 */

/* Enum for calorie checking mode */
typedef enum { IGNORE_CALORIES = 0, CHECK_CALORIES = 1 } CalorieMode;

/* Ingredient structure */
typedef struct {
  char name[16];
  int capacity;
  int durability;
  int flavor;
  int texture;
  int calories;
} Ingredient;

/* Recipe search state */
typedef struct {
  long best_score_part1;
  long best_score_part2;
} SearchState;

/* Property breakdown for debugging */
typedef struct {
  long capacity;
  long durability;
  long flavor;
  long texture;
  long calories;
} PropertyBreakdown;

/* Function prototypes */
void add_ingredient(Ingredient ingredients[], int *count, const char *name,
                    int capacity, int durability, int flavor, int texture,
                    int calories);
long calculate_score(const Ingredient ingredients[], int ingredient_count,
                     const int amounts[], CalorieMode mode);
void calculate_properties(const Ingredient ingredients[], int ingredient_count,
                          const int amounts[], PropertyBreakdown *props);
void print_property_breakdown(const int amounts[]);
void try_combinations(const Ingredient ingredients[], int ingredient_count,
                      int current_index, int remaining, int amounts[],
                      SearchState *state);
void find_optimal_recipe(const Ingredient ingredients[], int ingredient_count,
                         SearchState *state);
void run_tests(void);

/*
 * Add an ingredient to the list
 */
void add_ingredient(Ingredient ingredients[], int *count, const char *name,
                    int capacity, int durability, int flavor, int texture,
                    int calories) {
  if (*count >= MAX_INGREDIENTS) {
    printf("ERROR: TOO MANY INGREDIENTS\n");
    return;
  }

  /* Use snprintf for safe string copying with bounds checking */
  snprintf(ingredients[*count].name, sizeof(ingredients[*count].name), "%s",
           name);
  ingredients[*count].capacity = capacity;
  ingredients[*count].durability = durability;
  ingredients[*count].flavor = flavor;
  ingredients[*count].texture = texture;
  ingredients[*count].calories = calories;
  (*count)++;
}

/*
 * Calculate individual property values for a recipe
 */
void calculate_properties(const Ingredient ingredients[], int ingredient_count,
                          const int amounts[], PropertyBreakdown *props) {
  int i;

  props->capacity = 0;
  props->durability = 0;
  props->flavor = 0;
  props->texture = 0;
  props->calories = 0;

  for (i = 0; i < ingredient_count; i++) {
    props->capacity += (long)ingredients[i].capacity * amounts[i];
    props->durability += (long)ingredients[i].durability * amounts[i];
    props->flavor += (long)ingredients[i].flavor * amounts[i];
    props->texture += (long)ingredients[i].texture * amounts[i];
    props->calories += (long)ingredients[i].calories * amounts[i];
  }
}

/*
 * Calculate score for a given recipe
 * Returns the score if valid, or 0 if invalid
 */
long calculate_score(const Ingredient ingredients[], int ingredient_count,
                     const int amounts[], CalorieMode mode) {
  PropertyBreakdown props;

  calculate_properties(ingredients, ingredient_count, amounts, &props);

  /* Check calorie constraint for Part 2 */
  if (mode == CHECK_CALORIES && props.calories != CALORIE_TARGET) {
    return 0;
  }

  /* Set negative values to 0 */
  if (props.capacity < 0)
    props.capacity = 0;
  if (props.durability < 0)
    props.durability = 0;
  if (props.flavor < 0)
    props.flavor = 0;
  if (props.texture < 0)
    props.texture = 0;

  /* Calculate final score (multiply all four properties) */
  return props.capacity * props.durability * props.flavor * props.texture;
}

/*
 * Print property breakdown for test verification (DRY helper function)
 */
void print_property_breakdown(const int amounts[]) {
  long cap = amounts[0] * -1 + amounts[1] * 2;
  long dur = amounts[0] * -2 + amounts[1] * 3;
  long fla = amounts[0] * 6 + amounts[1] * -2;
  long tex = amounts[0] * 3 + amounts[1] * -1;

  printf("  DETAILS: CAP=%ld, DUR=%ld, FLA=%ld, TEX=%ld\n", cap, dur, fla, tex);
}

/*
 * Recursive function to try all combinations of ingredient amounts
 */
void try_combinations(const Ingredient ingredients[], int ingredient_count,
                      int current_index, int remaining, int amounts[],
                      SearchState *state) {
  long score;
  int amount;

  /* Base case: Last ingredient gets all remaining teaspoons */
  if (current_index == ingredient_count - 1) {
    amounts[current_index] = remaining;

    /* Calculate score for Part 1 (no calorie constraint) */
    score = calculate_score(ingredients, ingredient_count, amounts,
                            IGNORE_CALORIES);
    if (score > state->best_score_part1) {
      state->best_score_part1 = score;
    }

    /* Calculate score for Part 2 (with calorie constraint) */
    score =
        calculate_score(ingredients, ingredient_count, amounts, CHECK_CALORIES);
    if (score > state->best_score_part2) {
      state->best_score_part2 = score;
    }

    return;
  }

  /* Try different amounts for current ingredient */
  for (amount = 0; amount <= remaining; amount++) {
    amounts[current_index] = amount;
    try_combinations(ingredients, ingredient_count, current_index + 1,
                     remaining - amount, amounts, state);
  }
}

/*
 * Find the optimal recipe by trying all possible combinations
 */
void find_optimal_recipe(const Ingredient ingredients[], int ingredient_count,
                         SearchState *state) {
  int amounts[MAX_INGREDIENTS];

  /* Initialize amounts array to zero using memset */
  memset(amounts, 0, sizeof(amounts));

  /* Reset best scores */
  state->best_score_part1 = 0;
  state->best_score_part2 = 0;

  printf("EVALUATING %d INGREDIENTS...\n", ingredient_count);

  /* Start recursive search */
  try_combinations(ingredients, ingredient_count, 0, TOTAL_TEASPOONS, amounts,
                   state);
}

/*
 * Run test cases to validate the implementation
 */
void run_tests(void) {
  Ingredient ingredients[MAX_INGREDIENTS];
  int ingredient_count = 0;
  int amounts[MAX_INGREDIENTS];
  SearchState state;
  long score;
  long calories;
  int pass;

  printf("\n=== RUNNING TESTS ===\n\n");

  /* Add test ingredients */
  add_ingredient(ingredients, &ingredient_count, "Butterscotch", -1, -2, 6, 3,
                 8);
  add_ingredient(ingredients, &ingredient_count, "Cinnamon", 2, 3, -2, -1, 3);

  printf("TEST INGREDIENTS:\n");
  printf("  BUTTERSCOTCH: CAP -1, DUR -2, FLA 6, TEX 3, CAL 8\n");
  printf("  CINNAMON: CAP 2, DUR 3, FLA -2, TEX -1, CAL 3\n\n");

  /* Test Part 1: 44 Butterscotch, 56 Cinnamon */
  printf("PART 1 TEST:\n");
  printf("  44 BUTTERSCOTCH + 56 CINNAMON\n");
  amounts[0] = 44;
  amounts[1] = 56;
  score =
      calculate_score(ingredients, ingredient_count, amounts, IGNORE_CALORIES);
  printf("  EXPECTED SCORE: 62842880\n");
  printf("  ACTUAL SCORE:   %ld", score);

  /* Store conditional result in variable to avoid duplicate evaluation */
  pass = (score == 62842880L);
  printf(pass ? " (PASS)\n" : " (FAIL)\n");

  /* Use helper function to avoid code duplication */
  print_property_breakdown(amounts);

  /* Test Part 2: 40 Butterscotch, 60 Cinnamon */
  printf("\nPART 2 TEST:\n");
  printf("  40 BUTTERSCOTCH + 60 CINNAMON\n");
  amounts[0] = 40;
  amounts[1] = 60;

  /* Calculate calories */
  calories = 40L * 8 + 60L * 3;
  printf("  CALORIES: %ld (TARGET: %d)\n", calories, CALORIE_TARGET);

  score =
      calculate_score(ingredients, ingredient_count, amounts, IGNORE_CALORIES);
  printf("  EXPECTED SCORE: 57600000\n");
  printf("  ACTUAL SCORE:   %ld", score);

  pass = (score == 57600000L);
  printf(pass ? " (PASS)\n" : " (FAIL)\n");

  /* Use helper function to avoid code duplication */
  print_property_breakdown(amounts);

  /* Now find optimal recipe */
  printf("\n=== FINDING OPTIMAL RECIPE ===\n\n");
  find_optimal_recipe(ingredients, ingredient_count, &state);

  printf("PART 1 - BEST SCORE: %ld\n", state.best_score_part1);
  printf("  EXPECTED: 62842880");

  pass = (state.best_score_part1 == 62842880L);
  printf(pass ? " (PASS)\n" : " (FAIL)\n");

  printf("\nPART 2 - BEST SCORE (500 CAL): %ld\n", state.best_score_part2);
  printf("  EXPECTED: 57600000");

  pass = (state.best_score_part2 == 57600000L);
  printf(pass ? " (PASS)\n" : " (FAIL)\n");
}

/*
 * Main program
 */
int main(void) {
  printf("\n");
  printf("ADVENT OF CODE 2015 - DAY 15\n");
  printf("COOKIE RECIPE OPTIMIZER\n");
  printf("C64 EDITION\n");

  run_tests();

  printf("\n=== ALL TESTS COMPLETE ===\n");

  return 0;
}
