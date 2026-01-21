#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

/* Constants for the problem */
#define SAMPLE_LEN 16
#define SAMPLE_COLS 6

/* Sample input from the problem description */
static const char *sample_input[SAMPLE_LEN] = {
    "eedadn",
    "drvtee",
    "eandsr",
    "raavrd",
    "atevrs",
    "tsrnev",
    "sdttsa",
    "rasrtv",
    "nssdts",
    "ntnada",
    "svetve",
    "tesnvt",
    "vntsnd",
    "vrdear",
    "dvrsen",
    "enarar"
};



void run_tests(void);
void solve(const char **input, int num_rows, int num_cols);

int main(void) {
    bgcolor(COLOR_BLUE);
    bordercolor(COLOR_LIGHTBLUE);
    textcolor(COLOR_WHITE);
    clrscr();

    cprintf("ADVENT OF CODE 2016 - DAY 6\r\n");
    cprintf("SIGNALS AND NOISE\r\n");
    cprintf("==========================\r\n");

    run_tests();

    cprintf("\r\nPRESS ANY KEY TO EXIT.\r\n");
    cgetc();

    return 0;
}

void run_tests(void) {
    cprintf("\r\nRUNNING TESTS WITH SAMPLE DATA...\r\n");
    solve(sample_input, SAMPLE_LEN, SAMPLE_COLS);
}

void solve(const char **input, int num_rows, int num_cols) {
    /* Frequency table: columns x alphabet */
    static unsigned int frequency[SAMPLE_COLS][26];
    int r, c, i;
    char part1[SAMPLE_COLS + 1];
    char part2[SAMPLE_COLS + 1];

    /* Reset frequency table */
    memset(frequency, 0, sizeof(frequency));

    /* Fill frequency table */
    for (r = 0; r < num_rows; ++r) {
        for (c = 0; c < num_cols; ++c) {
            unsigned char char_idx = input[r][c] - 'a';
            if (char_idx < 26) {
                frequency[c][char_idx]++;
            }
        }
    }

    /* Find most and least common per column */
    for (c = 0; c < num_cols; ++c) {
        unsigned int max_count = 0;
        unsigned int min_count = UINT_MAX;
        int max_idx = 0;
        int min_idx = 0;

        for (i = 0; i < 26; ++i) {
            unsigned int count = frequency[c][i];
            
            /* Part 1: Most common */
            if (count > max_count) {
                max_count = count;
                max_idx = i;
            }

            /* Part 2: Least common (must occur at least once) */
            if (count > 0 && count < min_count) {
                min_count = count;
                min_idx = i;
            }
        }

        part1[c] = (char)('a' + max_idx);
        part2[c] = (char)('a' + min_idx);
    }
    part1[num_cols] = '\0';
    part2[num_cols] = '\0';

    cprintf("\r\nRESULTS:\r\n");
    cprintf("PART 1 (MOST COMMON): %s\r\n", part1);
    cprintf("EXPECTED:             easter\r\n");
    
    cprintf("\r\nPART 2 (LEAST COMMON): %s\r\n", part2);
    cprintf("EXPECTED:              advent\r\n");

    if (strcmp(part1, "easter") == 0 && strcmp(part2, "advent") == 0) {
        textcolor(COLOR_GREEN);
        cprintf("\r\nALL TESTS PASSED!\r\n");
    } else {
        textcolor(COLOR_RED);
        cprintf("\r\nTESTS FAILED!\r\n");
    }
    textcolor(COLOR_WHITE);
}
