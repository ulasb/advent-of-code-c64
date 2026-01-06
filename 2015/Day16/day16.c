/*
 * Advent of Code 2015 - Day 16 (C64 Edition)
 * Aunt Sue - Forensic Analysis
 *
 * This program identifies the correct Aunt Sue based on a list of properties.
 * Part 1: Exact match on known properties.
 * Part 2: Range match on specific properties (cats/trees > target,
 * pomeranians/goldfish < target).
 */

#include <stdio.h>

/* Constants */
#define UNKNOWN -1
#define MAX_SUES 10

/* Property comparison helpers */
#define CHECK_EQ(v, t)                                                         \
  if (v != UNKNOWN && v != t)                                                  \
    return 0;
#define CHECK_GT(v, t)                                                         \
  if (v != UNKNOWN && v <= t)                                                  \
    return 0;
#define CHECK_LT(v, t)                                                         \
  if (v != UNKNOWN && v >= t)                                                  \
    return 0;

/* Property definition macro (X-macro)
 * X(name, label, target_value, part2_rule)
 * part2_rule: EQ (equal), GT (greater than), LT (less than)
 */
#define PROPERTIES                                                             \
  X(children, "CHI", 3, EQ)                                                    \
  X(cats, "CAT", 7, GT)                                                        \
  X(samoyeds, "SAM", 2, EQ)                                                    \
  X(pomeranians, "POM", 3, LT)                                                 \
  X(akitas, "AKI", 0, EQ)                                                      \
  X(vizslas, "VIZ", 0, EQ)                                                     \
  X(goldfish, "GOL", 5, LT)                                                    \
  X(trees, "TRE", 3, GT)                                                       \
  X(cars, "CAR", 2, EQ)                                                        \
  X(perfumes, "PER", 1, EQ)

/* Sue structure */
typedef struct {
  int id; /* ID can be up to 500 */
#define X(name, label, target, rule) signed char name;
  PROPERTIES
#undef X
} Sue;

/* Function prototypes */
void init_sue(Sue *s, int id);
int check_part1(const Sue *s);
int check_part2(const Sue *s);
void print_sue(const Sue *s);
void run_tests(void);

/*
 * Initialize a Sue with unknown values
 */
void init_sue(Sue *s, int id) {
  s->id = id;
#define X(name, label, target, rule) s->name = UNKNOWN;
  PROPERTIES
#undef X
}

/*
 * Check if Sue matches Part 1 criteria (exact match)
 * Returns 1 if match, 0 if not.
 */
int check_part1(const Sue *s) {
#define X(name, label, target, rule) CHECK_EQ(s->name, target)
  PROPERTIES
#undef X
  return 1;
}

/*
 * Check if Sue matches Part 2 criteria (ranges)
 * Returns 1 if match, 0 if not.
 */
int check_part2(const Sue *s) {
#define X(name, label, target, rule) CHECK_##rule(s->name, target)
  PROPERTIES
#undef X
  return 1;
}

void print_sue(const Sue *s) {
  printf("SUE %d: ", s->id);
#define X(name, label, target, rule)                                           \
  if (s->name != UNKNOWN)                                                      \
    printf("%s %d ", label, (int)s->name);
  PROPERTIES
#undef X
  printf("\n");
}

void run_tests(void) {
  Sue test_sues[MAX_SUES];
  unsigned char i;
  unsigned char count = 0;
  Sue *s;

  printf("\n=== RUNNING TESTS ===\n\n");

  /* Sue 1: Matches Part 1 exactly */
  s = &test_sues[count++];
  init_sue(s, count);
  s->children = 3;
  s->cats = 7;
  s->samoyeds = 2;

  /* Sue 2: Fails Part 1 (cats mismatch) */
  s = &test_sues[count++];
  init_sue(s, count);
  s->cats = 8;
  s->trees = 9;

  /* Sue 3: Matches Part 2 (cats > 7, trees > 3) */
  s = &test_sues[count++];
  init_sue(s, count);
  s->cats = 8;
  s->trees = 4;
  s->children = 3; /* Exact match on regular prop */

  /* Sue 4: Matches Part 1 (exact match on all provided props) */
  s = &test_sues[count++];
  init_sue(s, count);
  s->goldfish = 5;
  s->perfumes = 1;

  /* Sue 5: Matches Part 2 (goldfish < 5, pomeranians < 3) */
  s = &test_sues[count++];
  init_sue(s, count);
  s->goldfish = 4;
  s->pomeranians = 2;

  printf("TARGETS:\n");
  printf("  CHI 3, CAT 7, SAM 2, POM 3, AKI 0\n");
  printf("  VIZ 0, GOL 5, TRE 3, CAR 2, PER 1\n\n");

  printf("EVALUATING SUES...\n\n");

  for (i = 0; i < count; i++) {
    print_sue(&test_sues[i]);

    /* Part 1 Check */
    if (check_part1(&test_sues[i])) {
      printf("  PART 1: MATCH (PASS)\n");
    } else {
      printf("  PART 1: NO MATCH\n");
    }

    /* Part 2 Check */
    if (check_part2(&test_sues[i])) {
      printf("  PART 2: MATCH (PASS)\n");
    } else {
      printf("  PART 2: NO MATCH\n");
    }
    printf("\n");
  }

  printf("VERIFICATION (EXPECTED RESULTS):\n");
  printf(
      "  SUE 1: Should match Part 1 and Part 2 (Part 2 rule EQ for these).\n");
  printf("  SUE 2: Should match NO PART.\n");
  printf("  SUE 3: Should match Part 2 only (Cats 8 > 7).\n");
  printf("  SUE 4: Should match Part 1 only (Goldfish 5 == 5, fails < 5).\n");
  printf("  SUE 5: Should match Part 2 only (Goldfish 4 < 5).\n");
}

int main(void) {
  printf("\n");
  printf("ADVENT OF CODE 2015 - DAY 16\n");
  printf("AUNT SUE FORENSICS\n");
  printf("C64 EDITION\n");

  run_tests();

  printf("\n=== TESTS COMPLETE ===\n");

  return 0;
}
