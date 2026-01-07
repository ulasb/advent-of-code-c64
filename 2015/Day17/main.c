/**
 * Advent of Code 2015 - Day 17
 * No Such Thing as Too Much
 *
 * Solution for Commodore 64 (cc65)
 */

#include <stdio.h>
#include <limits.h>

/* Configuration */
#define MAX_CONTAINERS 20

/* Global state
 * Using a simple array for containers.
 * For the sample input:
 * Containers: 20, 15, 10, 5, 5
 * Target: 25
 */
unsigned char containers[] = {20, 15, 10, 5, 5};
unsigned char num_containers = sizeof(containers) / sizeof(containers[0]);
unsigned int target = 25;

/* Results */
unsigned long part1_ans = 0;
unsigned long part2_ans = 0;
unsigned int min_containers = INT_MAX;

/**
 * Recursive solver
 * index: current container index
 * current_sum: sum of selected containers
 * count: number of selected containers
 */
void solve(unsigned char index, unsigned int current_sum, unsigned char count) {
    /* Base case: Exact match found */
    if (current_sum == target) {
        part1_ans++;
        
        /* Check for Part 2 (minimum containers) */
        if (count < min_containers) {
            min_containers = count;
            part2_ans = 1;
        } else if (count == min_containers) {
            part2_ans++;
        }
        return;
    }
    
    /* Base case: Sum exceeded or no more containers */
    if (current_sum > target || index >= num_containers) {
        return;
    }
    
    /* Branch 1: Include the current container */
    solve(index + 1, current_sum + containers[index], count + 1);
    
    /* Branch 2: Exclude the current container */
    solve(index + 1, current_sum, count);
}

int main(void) {
    printf("\n--- AoC 2015 Day 17 ---\n");
    printf("Target: %u liters\n", target);
    printf("Containers: ");
    {
        unsigned char i;
        for (i = 0; i < num_containers; ++i) {
            printf("%d ", containers[i]);
        }
    }
    printf("\n\nComputing...\n");

    /* Reset globals just in case */
    part1_ans = 0;
    min_containers = INT_MAX;
    part2_ans = 0;

    solve(0, 0, 0);

    printf("Part 1 Combinations: %lu\n", part1_ans);
    printf("Min Containers: %u\n", min_containers);
    printf("Part 2 Combinations: %lu\n", part2_ans);
    
    return 0;
}
