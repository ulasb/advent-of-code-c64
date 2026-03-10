#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 16
#define MAX_WORD_LEN 16

/* Global buffers for safety on C64 stack */
char global_words[MAX_WORDS][MAX_WORD_LEN];

/* Function Prototypes */
void run_tests(void);
int is_valid_part1(const char *phrase);
int is_valid_part2(const char *phrase);
void sort_string(char *s);
int split_words(const char *phrase);
int has_no_duplicates(int count);
void test_p1(const char *phrase, int expected);
void test_p2(const char *phrase, int expected);

int main(void) {
    bgcolor(COLOR_BLUE);
    bordercolor(COLOR_LIGHTBLUE);
    textcolor(COLOR_WHITE);
    clrscr();
    gotoxy(0, 0);

    cprintf("ADVENT OF CODE 2017 - DAY 4\r\n");
    cprintf("HIGH-ENTROPY PASSPHRASES\r\n");
    cprintf("============================\r\n");

    run_tests();

    cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
    cgetc();

    return 0;
}

void sort_string(char *s) {
    int n = strlen(s);
    int i, j, min_idx;
    char temp;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (s[j] < s[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            temp = s[i];
            s[i] = s[min_idx];
            s[min_idx] = temp;
        }
    }
}

int split_words(const char *phrase) {
    int count = 0;
    const char *start = phrase;
    const char *end;
    int len;

    while (*start != '\0' && count < MAX_WORDS) {
        /* Skip leading spaces */
        while (*start == ' ' && *start != '\0') start++;
        if (*start == '\0') break;

        /* Find end of word */
        end = start;
        while (*end != ' ' && *end != '\0') end++;

        len = end - start;
        if (len >= MAX_WORD_LEN) len = MAX_WORD_LEN - 1;

        memcpy(global_words[count], start, len);
        global_words[count][len] = '\0';
        count++;

        start = end;
    }
    return count;
}

int has_no_duplicates(int count) {
    int i, j;
    for (i = 0; i < count; i++) {
        for (j = i + 1; j < count; j++) {
            if (strcmp(global_words[i], global_words[j]) == 0) return 0;
        }
    }
    return 1;
}

int is_valid_part1(const char *phrase) {
    int count = split_words(phrase);
    if (count <= 0) return 0;
    return has_no_duplicates(count);
}

int is_valid_part2(const char *phrase) {
    int count = split_words(phrase);
    int i;

    if (count <= 0) return 0;

    /* Sort each word to detect anagrams */
    for (i = 0; i < count; i++) {
        sort_string(global_words[i]);
    }

    return has_no_duplicates(count);
}

void test_p1(const char *phrase, int expected) {
    int res = is_valid_part1(phrase);
    cprintf("P1: %s -> %s\r\n", phrase, (res == expected) ? "PASS" : "FAIL");
}

void test_p2(const char *phrase, int expected) {
    int res = is_valid_part2(phrase);
    cprintf("P2: %s -> %s\r\n", phrase, (res == expected) ? "PASS" : "FAIL");
}

void run_tests(void) {
    cprintf("\r\nPART 1 TESTS:\r\n");
    test_p1("aa bb cc dd ee", 1);
    test_p1("aa bb cc dd aa", 0);
    test_p1("aa bb cc dd aaa", 1);

    cprintf("\r\nPART 2 TESTS:\r\n");
    test_p2("abcde fghij", 1);
    test_p2("abcde xyz ecdab", 0);
    test_p2("a ab abc abd abf", 1);
    test_p2("oiii ioii iioi", 0);
}
