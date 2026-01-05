#include <conio.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Constants */
#define MAX_PEOPLE 10
#define NAME_LEN 16

/* Global Variables */
int happiness[MAX_PEOPLE][MAX_PEOPLE];
char people_names[MAX_PEOPLE][NAME_LEN];
int people_count = 0;
int current_order[MAX_PEOPLE];
int best_order[MAX_PEOPLE];
long max_happiness = LONG_MIN;

/* Function Prototypes */
void init_happiness(void);
int find_person(const char *name);
int add_person(const char *name);
void add_relation(const char *p1, const char *p2, int val);
void solve(void);
void permute(int start, int end);
long calculate_total_happiness(void);
void print_best_arrangement(void);

void init_happiness(void) {
  people_count = 0;
  max_happiness = LONG_MIN;

  memset(happiness, 0, sizeof(happiness));
}

int find_person(const char *name) {
  int i;
  for (i = 0; i < people_count; i++) {
    if (strcmp(people_names[i], name) == 0)
      return i;
  }
  return -1;
}

int add_person(const char *name) {
  int id = find_person(name);
  if (id == -1) {
    if (people_count >= MAX_PEOPLE)
      return -1;
    snprintf(people_names[people_count], NAME_LEN, "%s", name);
    return people_count++;
  }
  return id;
}

void add_relation(const char *p1, const char *p2, int val) {
  int id1 = add_person(p1);
  int id2 = add_person(p2);
  if (id1 != -1 && id2 != -1) {
    happiness[id1][id2] = val;
  }
}

/*
 * Calculate total happiness for a circular seating arrangement.
 * Each person i has two neighbors: (i-1) and (i+1).
 * We add the happiness 'current' feels sitting next to both.
 */
long calculate_total_happiness(void) {
  long total = 0;
  int i;
  for (i = 0; i < people_count; i++) {
    int left = (i - 1 + people_count) % people_count;
    int right = (i + 1) % people_count;
    int current = current_order[i];

    total += (long)happiness[current][current_order[left]];
    total += (long)happiness[current][current_order[right]];
  }
  return total;
}

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

void permute(int start, int end) {
  int i;
  if (start == end) {
    long current = calculate_total_happiness();
    if (current > max_happiness) {
      max_happiness = current;
      memcpy(best_order, current_order, sizeof(int) * people_count);
    }
    return;
  }

  for (i = start; i <= end; i++) {
    swap(&current_order[start], &current_order[i]);
    permute(start + 1, end);
    swap(&current_order[start], &current_order[i]); // backtrack
  }
}

/*
 * Entry point for finding the optimal arrangement.
 * To optimize, we fix the first person (index 0) and only permute
 * the remaining (n-1) people. This accounts for circular
 * rotation symmetry, reducing n! permutations to (n-1)!.
 */
void solve(void) {
  int i;
  for (i = 0; i < people_count; i++) {
    current_order[i] = i;
  }

  /* Start permuting from index 1, keeping index 0 fixed */
  permute(1, people_count - 1);
}

void print_best_arrangement(void) {
  int i;
  printf("Optimal: ");
  for (i = 0; i < people_count; i++) {
    printf("%s", people_names[best_order[i]]);
    if (i < people_count - 1)
      printf(" -> ");
  }
  printf("\nTotal Happiness: %ld\n", max_happiness);
}

void run_test_part1(void) {
  printf("\n--- Part 1 Test ---\n");
  init_happiness();

  add_relation("Alice", "Bob", 54);
  add_relation("Alice", "Carol", -79);
  add_relation("Alice", "David", -2);
  add_relation("Bob", "Alice", 83);
  add_relation("Bob", "Carol", -7);
  add_relation("Bob", "David", -63);
  add_relation("Carol", "Alice", -62);
  add_relation("Carol", "Bob", 60);
  add_relation("Carol", "David", 55);
  add_relation("David", "Alice", 46);
  add_relation("David", "Bob", -7);
  add_relation("David", "Carol", 41);

  solve();
  print_best_arrangement();

  if (max_happiness == 330) {
    printf("PART 1: (PASS)\n");
  } else {
    printf("PART 1: (FAIL) Expected 330\n");
  }
}

void run_test_part2(void) {
  printf("\n--- Part 2 Test (Including Myself) ---\n");

  /* Part 2: Add yourself with 0 happiness for all relationships */
  /* We can just add "Me" to the existing people_count */
  add_person("Me");
  /* No need to add relations for "Me" as happiness is initialized to 0 */

  /* Reset max_happiness to find new optimum */
  max_happiness = LONG_MIN;
  solve();
  print_best_arrangement();

  /* We don't have an expected value for Part 2 from the user,
     but we can verify the logic works. */
  printf("PART 2: Completed\n");
}

int main(void) {
  clrscr();
  printf("AoC 2015 Day 13: Seating Arrangement\n");
  printf("====================================\n");

  run_test_part1();
  run_test_part2();

  printf("\nPress any key to exit...\n");
  cgetc();

  return 0;
}
