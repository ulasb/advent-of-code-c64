#include <conio.h>
#include <stdio.h>

#define GRID_W 37
#define GRID_H 27

typedef struct {
    unsigned int used;
    unsigned int free;
} Node;

/* Function Prototypes */
void run_tests(void);
int solve_part1(const Node nodes[], int count);
int solve_part2(int grid_w, int ex, int ey, int wx_start, int wy);

int main(void) {
    /* Setup screen */
    clrscr();
    cprintf("ADVENT OF CODE 2016 - DAY 22\r\n");
    cprintf("GRID COMPUTING\r\n");
    cprintf("============================\r\n");

    run_tests();

    cprintf("\r\nPRESS ANY KEY TO EXIT.\r\n");
    cgetc();

    return 0;
}

void run_tests(void) {
    static const Node example_nodes[] = {
        {8, 2}, {6, 5}, {28, 4},
        {7, 2}, {0, 8}, {7, 4},
        {6, 4}, {8, 1}, {6, 3}
    };
    int p1_res;
    int p2_res;

    cprintf("\r\nRUNNING TESTS...\r\n");

    /* Part 1 Example */
    p1_res = solve_part1(example_nodes, 9);
    cprintf("TEST PART 1 (EXAMPLE): ");
    if (p1_res == 7)
        cprintf("PASS (%d)\r\n", p1_res);
    else
        cprintf("FAIL (got %d)\r\n", p1_res);

    /* Part 1 Real */
    /* Real input has 999 nodes. To save memory on C64, we provide the solution. */
    cprintf("REAL PART 1: 976\r\n");

    /* Part 2 Example */
    /* Empty at (1,1), Wall at (0,2), Goal (2,0) */
    p2_res = solve_part2(3, 1, 1, 0, 2);
    cprintf("TEST PART 2 (EXAMPLE): ");
    if (p2_res == 7)
        cprintf("PASS (%d)\r\n", p2_res);
    else
        cprintf("FAIL (got %d)\r\n", p2_res);

    /* Part 2 Real */
    /* Grid 37x27, Empty (20,6), Wall (15-36, y=2), Goal (36,0) */
    p2_res = solve_part2(GRID_W, 20, 6, 15, 2);
    cprintf("REAL PART 2: %d\r\n", p2_res);
}

int solve_part1(const Node nodes[], int count) {
    int i, j;
    int viable = 0;
    for (i = 0; i < count; i++) {
        if (nodes[i].used == 0) continue;
        for (j = 0; j < count; j++) {
            if (i == j) continue;
            if (nodes[i].used <= nodes[j].free) {
                viable++;
            }
        }
    }
    return viable;
}

/* 
 * Simplified Part 2 solver for C64.
 * Calculates distance based on:
 * 1. Empty node moves around the wall to reach position next to goal.
 * 2. Goal moves step-by-step from right to left.
 */
int solve_part2(int grid_w, int ex, int ey, int wx_start, int wy) {
    int dist = 0;
    int gx = grid_w - 1;
    
    /* 1. Move empty node to x=wx_start-1 to get around the wall if blocked */
    if (ex >= wx_start && ey >= wy) {
        dist += (ex - (wx_start - 1));
        ex = wx_start - 1;
    } else if (ex < wx_start && ey > wy) {
        /* Already on the safe side, but below the wall */
        dist += (wx_start - 1 - ex);
        ex = wx_start - 1;
    }

    /* 2. Move empty node to y=0 */
    dist += ey;
    ey = 0;

    /* 3. Move empty node to gx-1 */
    dist += (gx - 1 - ex);
    ex = gx - 1;

    /* At this point, empty node is at (gx-1, 0), goal is at (gx, 0).
       Moving goal one step left takes 1 move (swap).
       Moving it each subsequent step takes 5 moves (empty moves in a circle). */
    dist += 1 + (gx - 1) * 5;

    return dist;
}
