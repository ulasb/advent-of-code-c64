#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Range structure using 32-bit unsigned integers */
typedef struct {
    unsigned long start;
    unsigned long end;
} Range;

/* Sample IP ranges for demonstration and testing */
Range sample_ranges[] = {
    {0UL, 2UL},
    {4UL, 5UL},
    {1UL, 4UL},
    {10UL, 20UL},
    {15UL, 25UL},
    {30UL, 35UL}
};

#define SAMPLE_COUNT (sizeof(sample_ranges) / sizeof(Range))

/* Global working buffer to avoid stack issues on C64 */
Range working_ranges[SAMPLE_COUNT];

/* Function prototypes */
void run_tests(void);
void solve_puzzle(void);
int compare_ranges(const void *a, const void *b);
int merge_ranges(Range *r, int count);

int main(void) {
    /* Setup screen aesthetics like the reference project */
    bgcolor(COLOR_BLUE);
    bordercolor(COLOR_LIGHTBLUE);
    textcolor(COLOR_WHITE);
    clrscr();
    gotoxy(0, 0);

    cprintf("ADVENT OF CODE 2016 - DAY 20\r\n");
    cprintf("FIREWALL RULES\r\n");
    cprintf("============================\r\n");

    run_tests();
    solve_puzzle();

    cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
    cgetc();

    return 0;
}

/* Comparator for qsort: sort by start address */
int compare_ranges(const void *a, const void *b) {
    const Range *ra = (const Range *)a;
    const Range *rb = (const Range *)b;
    if (ra->start < rb->start) return -1;
    if (ra->start > rb->start) return 1;
    return 0;
}

/* Merge overlapping and adjacent ranges in-place */
int merge_ranges(Range *r, int count) {
    int i;
    int merged_count = 0;
    if (count == 0) return 0;

    /* Sort ranges by start value */
    qsort(r, count, sizeof(Range), compare_ranges);

    merged_count = 1;
    for (i = 1; i < count; i++) {
        Range *last = &r[merged_count - 1];
        Range *curr = &r[i];

        /* If current starts within last (or immediately after) */
        if (curr->start <= last->end + 1) {
            /* Extend last if current ends further */
            if (curr->end > last->end) {
                last->end = curr->end;
            }
        } else {
            /* No overlap, add as new range */
            r[merged_count] = *curr;
            merged_count++;
        }
    }
    return merged_count;
}

void run_tests(void) {
    Range test_ranges[3];
    int count = 3;
    int merged;
    int i;
    unsigned long first_allowed = 0;

    cprintf("\r\nRUNNING TESTS...\r\n");

    /* Test Case: 5-8, 0-2, 4-7 */
    test_ranges[0].start = 5; test_ranges[0].end = 8;
    test_ranges[1].start = 0; test_ranges[1].end = 2;
    test_ranges[2].start = 4; test_ranges[2].end = 7;

    merged = merge_ranges(test_ranges, count);
    
    /* Find first gap */
    first_allowed = 0;
    for (i = 0; i < merged; i++) {
        if (first_allowed < test_ranges[i].start) {
            break;
        }
        first_allowed = test_ranges[i].end + 1;
    }

    cprintf("TEST PART 1 (Example): ");
    if (first_allowed == 3) {
        cprintf("PASS\r\n");
    } else {
        cprintf("FAIL (got %lu)\r\n", first_allowed);
    }
}

void solve_puzzle(void) {
    int i;
    int merged;
    unsigned long first_allowed = 0;
    unsigned long allowed_count = 0;
    unsigned long blocked_count = 0;

    cprintf("\r\nSOLVING SAMPLE (%d ranges)...\r\n", SAMPLE_COUNT);

    /* Copy to working buffer to preserve original data and avoid stack usage */
    memcpy(working_ranges, sample_ranges, sizeof(sample_ranges));
    merged = merge_ranges(working_ranges, SAMPLE_COUNT);

    /* Part 1: First allowed IP */
    first_allowed = 0;
    for (i = 0; i < merged; i++) {
        if (first_allowed < working_ranges[i].start) {
            break;
        }
        first_allowed = working_ranges[i].end + 1;
    }
    cprintf("PART 1 (First Allowed): %lu\r\n", first_allowed);

    /* Part 2: Total number of allowed IPs */
    /* Total addresses = 2^32 = 4294967296 */
    /* We count allowed by subtracting blocked from total */
    for (i = 0; i < merged; i++) {
        blocked_count += (working_ranges[i].end - working_ranges[i].start + 1);
    }
    
    /* Allowed = 4294967296 - blocked_count */
    allowed_count = (4294967295UL - blocked_count) + 1;

    cprintf("PART 2 (Total Allowed): %lu\r\n", allowed_count);
    
    /* Verification for sample data */
    if (first_allowed == 6 && allowed_count == 4294967268UL) {
        cprintf("SAMPLE VERIFIED!\r\n");
    }
}
