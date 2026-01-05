#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Advent of Code 2015 - Day 12: JSAbacusFramework.io
 * C64 implementation using cc65
 * Optimized with split arrays to avoid struct size limitations
 */

#define MAX_STACK 32

/* Stack data split into multiple arrays for C64/cc65 compatibility */
char stack_type[MAX_STACK];
long stack_sum[MAX_STACK];
char stack_red[MAX_STACK];
char stack_expect[MAX_STACK];
int stack_ptr = 0;

void push_context(char type) {
  if (stack_ptr >= MAX_STACK) {
    fprintf(stderr, "Error: Stack overflow\n");
    exit(1);
  }
  stack_type[stack_ptr] = type;
  stack_sum[stack_ptr] = 0;
  stack_red[stack_ptr] = 0;
  /* In arrays, everything is a value. In objects, we wait for ':' */
  stack_expect[stack_ptr] = (type == '[');
  stack_ptr++;
}

/* Part 1: Just sum all numbers in the string */
long solve_part1(const char *s) {
  long total = 0;
  char *endptr;
  while (*s) {
    if (isdigit((unsigned char)*s) || (*s == '-' && isdigit((unsigned char)*(s + 1)))) {
      total += strtol(s, &endptr, 10);
      s = endptr;
    } else {
      s++;
    }
  }
  return total;
}

/* Part 2: Sum numbers, but ignore objects containing the value "red" */
long solve_part2(const char *s) {
  const char *start;
  int len;
  char *endptr;
  long val;
  long current_sum;
  char current_red;

  stack_ptr = 0;
  push_context('['); /* Root container */

  while (*s) {
    if (*s == '{') {
      push_context('{');
      s++;
    } else if (*s == '[') {
      push_context('[');
      s++;
    } else if (*s == '}') {
      if (stack_ptr > 0 && stack_type[stack_ptr - 1] == '{') {
        stack_ptr--;
        current_sum = stack_sum[stack_ptr];
        current_red = stack_red[stack_ptr];
        if (!current_red && stack_ptr > 0) {
          stack_sum[stack_ptr - 1] += current_sum;
        }
      }
      s++;
    } else if (*s == ']') {
      if (stack_ptr > 0 && stack_type[stack_ptr - 1] == '[') {
        stack_ptr--;
        current_sum = stack_sum[stack_ptr];
        if (stack_ptr > 0) {
          stack_sum[stack_ptr - 1] += current_sum;
        }
      }
      s++;
    } else if (*s == ':') {
      if (stack_ptr > 0 && stack_type[stack_ptr - 1] == '{') {
        stack_expect[stack_ptr - 1] = 1;
      }
      s++;
    } else if (*s == ',') {
      if (stack_ptr > 0 && stack_type[stack_ptr - 1] == '{') {
        stack_expect[stack_ptr - 1] = 0;
      }
      s++;
    } else if (*s == '"') {
      /* Handle strings */
      s++;
      start = s;
      while (*s && *s != '"') {
        if (*s == '\\') {
          s++; /* Skip escaped char */
          if (*s == '\0')
            break;
        }
        s++;
      }
      len = (int)(s - start);

      /* Check for "red" as a value in an object */
      if (stack_ptr > 0 && stack_type[stack_ptr - 1] == '{' &&
          stack_expect[stack_ptr - 1]) {
        if (len == 3 && strncmp(start, "red", 3) == 0) {
          stack_red[stack_ptr - 1] = 1;
        }
      }
      if (*s == '"')
        s++;
    } else if (isdigit((unsigned char)*s) || (*s == '-' && isdigit((unsigned char)*(s + 1)))) {
      val = strtol(s, &endptr, 10);
      if (stack_ptr > 0) {
        stack_sum[stack_ptr - 1] += val;
      }
      s = endptr;
    } else {
      s++;
    }
  }
  return stack_sum[0];
}

void run_test(const char *label, const char *json, long expected1,
              long expected2) {
  long r1 = solve_part1(json);
  long r2 = solve_part2(json);
  printf("%s\nJSON: %s\n", label, json);
  printf("P1: %ld (Exp: %ld) %s\n", r1, expected1,
         (r1 == expected1 ? "PASS" : "FAIL"));
  printf("P2: %ld (Exp: %ld) %s\n", r2, expected2,
         (r2 == expected2 ? "PASS" : "FAIL"));
  printf("--------------------------\n");
}

int main() {
  printf("AOC 2015 Day 12 - JSAbacus\n");
  printf("Running validation tests...\n\n");

  /* Part 1 Tests */
  run_test("T1", "[1,2,3]", 6, 6);
  run_test("T2", "{\"a\":2,\"b\":4}", 6, 6);
  run_test("T3", "[[[3]]]", 3, 3);
  run_test("T4", "{\"a\":{\"b\":4},\"c\":-1}", 3, 3);
  run_test("T5", "{\"a\":[-1,1]}", 0, 0);
  run_test("T6", "[-1,{\"a\":1}]", 0, 0);
  run_test("T7", "[]", 0, 0);
  run_test("T8", "{}", 0, 0);

  /* Part 2 Specific Tests */
  run_test("T9", "[1,{\"c\":\"red\",\"b\":2},3]", 6, 4);
  run_test("T10", "{\"d\":\"red\",\"e\":[1,2,3,4],\"f\":5}", 15, 0);
  run_test("T11", "[1,\"red\",5]", 6, 6);

  printf("Tests completed.\n");
  return 0;
}
