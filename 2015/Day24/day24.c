/*
 * Advent of Code 2015 - Day 24: It Hangs in the Balance
 * C64 Edition using cc65
 * 
 * This program finds the ideal configuration for balancing packages.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

#define MAX_WEIGHTS 32

/* 64-bit unsigned integer structure and math for Quantum Entanglement */
typedef struct {
    unsigned long low;
    unsigned long high;
} uint64_t_c64;

void set64(uint64_t_c64* a, unsigned long low, unsigned long high) {
    a->low = low;
    a->high = high;
}

void copy64(uint64_t_c64* dest, uint64_t_c64* src) {
    dest->low = src->low;
    dest->high = src->high;
}

/* 64-bit multiplication: a = a * b */
void mul64(uint64_t_c64* a, unsigned int b) {
    unsigned long r0, r1, r2, r3;
    unsigned long a0 = a->low & 0xFFFF;
    unsigned long a1 = (a->low >> 16) & 0xFFFF;
    unsigned long a2 = a->high & 0xFFFF;
    unsigned long a3 = (a->high >> 16) & 0xFFFF;

    r0 = a0 * b;
    r1 = a1 * b + (r0 >> 16);
    r2 = a2 * b + (r1 >> 16);
    r3 = a3 * b + (r2 >> 16);

    a->low = (r0 & 0xFFFF) | ((r1 & 0xFFFF) << 16);
    a->high = (r2 & 0xFFFF) | ((r3 & 0xFFFF) << 16);
}

/* Compare two 64-bit integers: -1 if a < b, 1 if a > b, 0 if equal */
int compare64(uint64_t_c64* a, uint64_t_c64* b) {
    if (a->high < b->high) return -1;
    if (a->high > b->high) return 1;
    if (a->low < b->low) return -1;
    if (a->low > b->low) return 1;
    return 0;
}

/* Convert uint64 to decimal string */
void format64(uint64_t_c64* a, char* buf) {
    char temp[21];
    int i = 0;
    uint64_t_c64 val;
    val.low = a->low;
    val.high = a->high;

    if (val.low == 0 && val.high == 0) {
        strcpy(buf, "0");
        return;
    }
    while (val.low != 0 || val.high != 0) {
        unsigned long rem = 0;
        unsigned long n3 = (val.high >> 16) & 0xFFFF;
        unsigned long n2 = val.high & 0xFFFF;
        unsigned long n1 = (val.low >> 16) & 0xFFFF;
        unsigned long n0 = val.low & 0xFFFF;
        
        unsigned long q3, q2, q1, q0;
        
        q3 = n3 / 10;
        rem = n3 % 10;
        q2 = (rem * 65536L + n2) / 10;
        rem = (rem * 65536L + n2) % 10;
        q1 = (rem * 65536L + n1) / 10;
        rem = (rem * 65536L + n1) % 10;
        q0 = (rem * 65536L + n0) / 10;
        rem = (rem * 65536L + n0) % 10;
        
        temp[i++] = (char)('0' + rem);
        val.high = (q3 << 16) | (q2 & 0xFFFF);
        val.low = (q1 << 16) | (q0 & 0xFFFF);
    }
    temp[i] = '\0';
    {
        int j;
        for (j = 0; j < i; ++j) {
            buf[j] = temp[i - 1 - j];
        }
        buf[i] = '\0';
    }
}

unsigned int weights[MAX_WEIGHTS];
unsigned char n_weights;
uint64_t_c64 best_qe;
unsigned char found_any;
unsigned long target_weight;

/* Recursive combination search with pruning */
void find_combinations(unsigned char start_idx, unsigned char count, unsigned long current_target, uint64_t_c64* current_qe) {
    unsigned char i;
    if (count == 0) {
        if (current_target == 0) {
            if (!found_any || compare64(current_qe, &best_qe) < 0) {
                copy64(&best_qe, current_qe);
                found_any = 1;
            }
        }
        return;
    }

    for (i = start_idx; i <= n_weights - count; ++i) {
        unsigned int w = weights[i];
        if (w > current_target) continue;
        
        /* Strong prune: if even 'count' largest remaining (weights[i]) can't reach target */
        if ((unsigned long)w * count < current_target) break;

        {
            uint64_t_c64 next_qe;
            copy64(&next_qe, current_qe);
            mul64(&next_qe, w);
            
            /* Prune: if current product is already worse than best found */
            if (found_any && compare64(&next_qe, &best_qe) >= 0) continue;

            find_combinations(i + 1, count - 1, current_target - w, &next_qe);
        }
    }
}

void solve(unsigned char num_groups) {
    unsigned long total_weight = 0;
    unsigned char r, i;
    char buf[32];
    clock_t start_time, end_time;

    for (i = 0; i < n_weights; ++i) total_weight += weights[i];
    target_weight = total_weight / num_groups;

    printf("Groups: %d, Target: %lu\n", num_groups, target_weight);
    
    start_time = clock();
    found_any = 0;
    /* Try increasing group sizes to find the absolute minimum number of packages */
    for (r = 1; r <= n_weights; ++r) {
        uint64_t_c64 start_qe;
        printf("Size %d... ", r);
        set64(&start_qe, 1, 0);
        find_combinations(0, r, target_weight, &start_qe);
        
        if (found_any) {
            end_time = clock();
            format64(&best_qe, buf);
            printf("\nQE: %s\n", buf);
            printf("Time: %lu ticks\n", (unsigned long)(end_time - start_time));
            return;
        }
        printf("not found\n");
    }
}

int main() {
    /* Example weights from the problem description */
    unsigned int example_weights[] = {11, 10, 9, 8, 7, 5, 4, 3, 2, 1};
    
    /* Typical real input data (first 30 primes/values approx) */
    unsigned int real_weights[] = {
        113, 109, 107, 103, 101, 97, 89, 83, 79, 73, 71, 67, 61, 59, 53, 47, 43, 41, 37, 31, 29, 23, 19, 17, 13, 11, 7, 5, 3, 1
    };

    clrscr();
    printf("AoC 2015 Day 24 - C64\n");
    printf("--------------------\n");

    /* First run example data */
    printf("Example Data (10 pkgs):\n");
    n_weights = sizeof(example_weights) / sizeof(example_weights[0]);
    if (n_weights > MAX_WEIGHTS) {
        printf("Error: too many example weights!\n");
        cgetc();
        return 1;
    }
    memcpy(weights, example_weights, n_weights * sizeof(unsigned int));
    solve(3);

    printf("\nPress any key for Real Data...\n");
    cgetc();

    /* Then run real data simulation */
    printf("\nReal Data Simulation (30 pkgs):\n");
    n_weights = sizeof(real_weights) / sizeof(real_weights[0]);
    if (n_weights > MAX_WEIGHTS) {
        printf("Error: too many real weights!\n");
        cgetc();
        return 1;
    }
    memcpy(weights, real_weights, n_weights * sizeof(unsigned int));
    
    printf("\nPart 1 (3 groups):\n");
    solve(3);
    
    printf("\nPart 2 (4 groups):\n");
    solve(4);

    printf("\nDone. Press any key.\n");
    cgetc();
    return 0;
}
