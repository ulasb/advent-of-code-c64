#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Function Prototypes --- */
long find_distance(long goal);
unsigned long find_first_larger(long goal);
void run_tests(void);

/* --- Constants --- */
#define GRID_SIZE 21
#define OFFSET 10

/* --- Global Variables --- */
/* Move large arrays to global scope to avoid stack overflow on C64 */
unsigned long spiral_grid[GRID_SIZE][GRID_SIZE];

/* 
 * PART 1: Manhattan Distance
 * Calculates the distance to square 1 in a spiral grid.
 */
long find_distance(long goal) {
    long ring = 0;
    long side = 1;
    long max_val;
    long i;
    long min_dist;
    long d;
    long center;

    if (goal == 1) return 0;

    /* Find which ring the goal belongs to */
    while (side * side < goal) {
        side += 2;
        ring++;
    }

    max_val = side * side;
    
    /* The centers of the four sides of the ring:
       centers[k] = max_val - ring - k * (side - 1) for k=0,1,2,3 */
    min_dist = 1000000L; 
    
    for (i = 0; i < 4; i++) {
        center = max_val - ring - i * (side - 1);
        d = goal - center;
        if (d < 0) d = -d;
        if (d < min_dist) min_dist = d;
    }

    return ring + min_dist;
}

/* 
 * PART 2: Sum of Adjacents 
 * Populates grid with sum of neighbors until value exceeds goal.
 */
unsigned long find_first_larger(long goal) {
    int x = 0, y = 0;
    int dx = 1, dy = 0;
    int nx, ny;
    unsigned long val;
    int temp;

    /* Initialize grid with zeros */
    memset(spiral_grid, 0, sizeof(spiral_grid));

    /* Start at center (square 1) */
    spiral_grid[x + OFFSET][y + OFFSET] = 1;

    while (1) {
        /* Move to next square */
        x += dx;
        y += dy;

        /* Safety check for grid boundaries */
        if (x + OFFSET < 0 || x + OFFSET >= GRID_SIZE || 
            y + OFFSET < 0 || y + OFFSET >= GRID_SIZE) {
            return 0; 
        }

        /* Calculate sum of all 8 neighbors */
        val = 0;
        for (nx = x - 1; nx <= x + 1; nx++) {
            for (ny = y - 1; ny <= y + 1; ny++) {
                val += spiral_grid[nx + OFFSET][ny + OFFSET];
            }
        }

        spiral_grid[x + OFFSET][y + OFFSET] = val;

        /* Check if we exceeded the goal */
        if (val > (unsigned long)goal) {
            return val;
        }

        /* Change direction if the square to the left is vacant.
           If moving (dx, dy), the left direction is (-dy, dx). */
        if (spiral_grid[x - dy + OFFSET][y + dx + OFFSET] == 0) {
            temp = dx;
            dx = -dy;
            dy = temp;
        }
    }
}

void run_tests(void) {
    long res_p1;
    unsigned long res_p2;

    cprintf("\r\nRUNNING TESTS...\r\n");

    res_p1 = find_distance(1L);
    cprintf("PART 1 (1): %ld (exp 0) ", res_p1);
    if (res_p1 == 0) cprintf("OK\r\n"); else cprintf("FAIL\r\n");

    res_p1 = find_distance(12L);
    cprintf("PART 1 (12): %ld (exp 3) ", res_p1);
    if (res_p1 == 3) cprintf("OK\r\n"); else cprintf("FAIL\r\n");

    res_p1 = find_distance(23L);
    cprintf("PART 1 (23): %ld (exp 2) ", res_p1);
    if (res_p1 == 2) cprintf("OK\r\n"); else cprintf("FAIL\r\n");

    res_p1 = find_distance(1024L);
    cprintf("PART 1 (1024): %ld (exp 31) ", res_p1);
    if (res_p1 == 31) cprintf("OK\r\n"); else cprintf("FAIL\r\n");

    res_p2 = find_first_larger(1L);
    cprintf("\r\nPART 2 (G>1): %lu (exp 2) ", res_p2);
    if (res_p2 == 2) cprintf("OK\r\n"); else cprintf("FAIL\r\n");

    res_p2 = find_first_larger(10L);
    cprintf("PART 2 (G>10): %lu (exp 11) ", res_p2);
    if (res_p2 == 11) cprintf("OK\r\n"); else cprintf("FAIL\r\n");

    res_p2 = find_first_larger(100L);
    cprintf("PART 2 (G>100): %lu (exp 122) ", res_p2);
    if (res_p2 == 122) cprintf("OK\r\n"); else cprintf("FAIL\r\n");

    res_p2 = find_first_larger(800L);
    cprintf("PART 2 (G>800): %lu (exp 806) ", res_p2);
    if (res_p2 == 806) cprintf("OK\r\n"); else cprintf("FAIL\r\n");
}

int main(void) {
    long goal = 277678L;
    
    bgcolor(COLOR_BLUE);
    bordercolor(COLOR_LIGHTBLUE);
    textcolor(COLOR_WHITE);
    clrscr();
    gotoxy(0, 0);

    cprintf("ADVENT OF CODE 2017 - DAY 3\r\n");
    cprintf("SPIRAL MEMORY\r\n");
    cprintf("============================\r\n");

    run_tests();

    cprintf("\r\n--- FINAL RESULTS ---\r\n");
    cprintf("GOAL: %ld\r\n", goal);
    cprintf("PART 1 DISTANCE: %ld\r\n", find_distance(goal));
    cprintf("PART 2 LARGER:   %lu\r\n", find_first_larger(goal));

    cprintf("\r\nPRESS ANY KEY TO EXIT.\r\n");
    cgetc();

    return 0;
}
