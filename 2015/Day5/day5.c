#include <conio.h>
#include <stdio.h>
#include <string.h>

// Test inputs for Part 1
#define PART1_TEST_COUNT 5
const char* part1_tests[PART1_TEST_COUNT] = {
    "ugknbfddgicrmopn",    // nice: 3+ vowels, double letter, no forbidden
    "aaa",                 // nice: 3+ vowels, double letter
    "jchzalrnumimnmhp",    // naughty: no double letter
    "haegwjzuvuyypxyu",    // naughty: contains xy
    "dvszwmarrgswjxmb"     // naughty: only 1 vowel
};

const int part1_expected[PART1_TEST_COUNT] = {1, 1, 0, 0, 0};  // 1 = nice, 0 = naughty

// Test inputs for Part 2
#define PART2_TEST_COUNT 4
const char* part2_tests[PART2_TEST_COUNT] = {
    "qjhvhtzxzqqjkmpb",    // nice: repeated pair (qj), letter with gap (zxz)
    "xxyxx",               // nice: repeated pair (xx), letter with gap (xyx)
    "uurcxstgmygtbstg",    // naughty: has pair (tg) but no gap repeat
    "ieodomkazucvgmuy"     // naughty: has gap repeat (odo) but no repeated pair
};

const int part2_expected[PART2_TEST_COUNT] = {1, 1, 0, 0};  // 1 = nice, 0 = naughty

int is_vowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int contains_forbidden_pair(const char* str) {
    size_t len = strlen(str);
    size_t i;

    for (i = 0; i < len - 1; ++i) {
        if ((str[i] == 'a' && str[i+1] == 'b') ||
            (str[i] == 'c' && str[i+1] == 'd') ||
            (str[i] == 'p' && str[i+1] == 'q') ||
            (str[i] == 'x' && str[i+1] == 'y')) {
            return 1;
        }
    }
    return 0;
}

int string_is_nice_part1(const char* str) {
    size_t len = strlen(str);
    size_t i;
    int vowel_count = 0;
    int has_double = 0;

    // Check for forbidden pairs
    if (contains_forbidden_pair(str)) {
        return 0;
    }

    if (len == 0) return 0;

    for (i = 0; i < len; i++) {
        // Count vowels
        if (is_vowel(str[i])) {
            vowel_count++;
        }

        // Check for double letters
        if (i > 0 && str[i] == str[i-1]) {
            has_double = 1;
        }
    }

    return (vowel_count >= 3) && has_double;
}

int has_repeated_pair(const char* str) {
    size_t len = strlen(str);
    size_t i, j;

    for (i = 0; i < len - 1; i++) {
        char pair[3];
        pair[0] = str[i];
        pair[1] = str[i+1];
        pair[2] = '\0';

        // Look for this pair appearing again later (without overlapping)
        for (j = i + 2; j < len - 1; ++j) {
            if (str[i] == str[j] && str[i+1] == str[j+1]) {
                return 1;
            }
        }
    }
    return 0;
}

int has_letter_with_gap(const char* str) {
    size_t len = strlen(str);
    size_t i;

    for (i = 0; i < len - 2; i++) {
        if (str[i] == str[i+2]) {
            return 1;
        }
    }
    return 0;
}

int string_is_nice_part2(const char* str) {
    return has_repeated_pair(str) && has_letter_with_gap(str);
}

void test_part1(void) {
    size_t i;
    int result;

    cprintf("Part 1 Tests:\r\n");
    cprintf("============\r\n");

    for (i = 0; i < PART1_TEST_COUNT; i++) {
        result = string_is_nice_part1(part1_tests[i]);
        cprintf("Input: %s -> %s", part1_tests[i], result ? "NICE" : "NAUGHTY");

        if (result == part1_expected[i]) {
            cprintf(" (PASS)\r\n");
        } else {
            cprintf(" (FAIL - expected %s)\r\n", part1_expected[i] ? "NICE" : "NAUGHTY");
        }
    }
    cprintf("\r\n");
}

void test_part2(void) {
    size_t i;
    int result;

    cprintf("Part 2 Tests:\r\n");
    cprintf("============\r\n");

    for (i = 0; i < PART2_TEST_COUNT; i++) {
        result = string_is_nice_part2(part2_tests[i]);
        cprintf("Input: %s -> %s", part2_tests[i], result ? "NICE" : "NAUGHTY");

        if (result == part2_expected[i]) {
            cprintf(" (PASS)\r\n");
        } else {
            cprintf(" (FAIL - expected %s)\r\n", part2_expected[i] ? "NICE" : "NAUGHTY");
        }
    }
    cprintf("\r\n");
}

int main(void) {
    clrscr();

    cprintf("Advent of Code 2015 - Day 5\r\n");
    cprintf("===========================\r\n\r\n");

    test_part1();
    test_part2();

    cprintf("Press any key to exit...\r\n");
    cgetc();

    return 0;
}
