#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Advent of Code 2016 - Day 24: Air Duct Spelunking
 * C64 Edition
 */

#define MAX_POINTS 8
#define MAX_QUEUE 256
#define MAX_GRID_WIDTH 180
#define MAX_GRID_HEIGHT 40

typedef struct {
    signed char x, y;
} Point;

typedef struct {
    signed char x, y;
    int dist;
} QueueItem;

/* Global data to stay off the stack and fit in BSS */
char grid[MAX_GRID_HEIGHT][MAX_GRID_WIDTH + 1];
Point points[MAX_POINTS];
int distances[MAX_POINTS][MAX_POINTS];
unsigned char visited[MAX_GRID_HEIGHT][MAX_GRID_WIDTH];
QueueItem queue[MAX_QUEUE];
int num_points;
int grid_w, grid_h;

/* TSP results */
int min_dist_p1;
int min_dist_p2;

/* Example data from problem description */
const char* example_grid[] = {
    "###########",
    "#0.1.....2#",
    "#.#######.#",
    "#4.......3#",
    "###########"
};
#define EXAMPLE_W 11
#define EXAMPLE_H 5

/* BFS to find shortest distance between two numbered points */
int get_distance(int p1_idx, int p2_idx) {
    int head = 0, tail = 0;
    signed char dx[] = {0, 0, 1, -1};
    signed char dy[] = {1, -1, 0, 0};
    int i;
    Point start;
    Point end;
    
    start = points[p1_idx];
    end = points[p2_idx];
    
    /* Clear visited array for current grid dimensions */
    for (i = 0; i < grid_h; i++) {
        memset(visited[i], 0, grid_w);
    }
    
    /* Initialize queue */
    queue[tail].x = start.x;
    queue[tail].y = start.y;
    queue[tail].dist = 0;
    tail++;
    
    visited[start.y][start.x] = 1;
    
    while (head != tail) {
        QueueItem current;
        current = queue[head];
        head = (head + 1) % MAX_QUEUE;
        
        if (current.x == end.x && current.y == end.y) {
            return current.dist;
        }
        
        for (i = 0; i < 4; i++) {
            signed char nx = current.x + dx[i];
            signed char ny = current.y + dy[i];
            
            /* Bounds and wall check */
            if (nx >= 0 && nx < grid_w && ny >= 0 && ny < grid_h &&
                grid[ny][nx] != '#' && !visited[ny][nx]) {
                
                visited[ny][nx] = 1;
                queue[tail].x = nx;
                queue[tail].y = ny;
                queue[tail].dist = current.dist + 1;
                tail = (tail + 1) % MAX_QUEUE;
                
                /* Check for queue overflow */
                if (tail == head) {
                    return -2; /* Overflow indicator */
                }
            }
        }
    }
    
    return -1; /* No path found */
}

/* Helper to swap integers in array */
void swap_indices(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* Recursive TSP solver using permutations */
void solve_tsp_recursive(int *a, int l, int r, int return_to_zero) {
    int i;
    int total;
    if (l == r) {
        /* Path starts at '0' then follows permutation in 'a' */
        total = distances[0][a[0]];
        for (i = 0; i < r; i++) {
            total += distances[a[i]][a[i+1]];
        }
        
        if (return_to_zero) {
            /* Return to starting point '0' */
            total += distances[a[r]][0];
            if (total < min_dist_p2) min_dist_p2 = total;
        } else {
            /* End at the last visited point */
            if (total < min_dist_p1) min_dist_p1 = total;
        }
    } else {
        for (i = l; i <= r; i++) {
            swap_indices((a + l), (a + i));
            solve_tsp_recursive(a, l + 1, r, return_to_zero);
            swap_indices((a + l), (a + i));
        }
    }
}

/* Run the example case */
void run_example(void) {
    int i, j;
    int p_indices[MAX_POINTS];
    
    cprintf("TEST: EXAMPLE GRID (%dx%d)\r\n", EXAMPLE_W, EXAMPLE_H);
    grid_w = EXAMPLE_W;
    grid_h = EXAMPLE_H;
    num_points = 0;
    
    /* Load example into grid buffer and identify points */
    for (i = 0; i < grid_h; i++) {
        strcpy(grid[i], example_grid[i]);
        for (j = 0; j < grid_w; j++) {
            if (grid[i][j] >= '0' && grid[i][j] <= '9') {
                int val = grid[i][j] - '0';
                points[val].x = j;
                points[val].y = i;
                if (val >= num_points) num_points = val + 1;
            }
        }
    }
    
    cprintf("FOUND %d POINTS.\r\n", num_points);
    cprintf("CALCULATING BFS DISTANCES...\r\n");
    for (i = 0; i < num_points; i++) {
        for (j = i + 1; j < num_points; j++) {
            int d = get_distance(i, j);
            distances[i][j] = d;
            distances[j][i] = d;
        }
    }
    
    /* Initialize permutation indices (excluding starting point 0) */
    for (i = 0; i < num_points - 1; i++) {
        p_indices[i] = i + 1;
    }
    
    min_dist_p1 = 32767;
    min_dist_p2 = 32767;
    
    cprintf("SOLVING TSP PERMUTATIONS...\r\n");
    
    /* Part 1: Visit all points once */
    solve_tsp_recursive(p_indices, 0, num_points - 2, 0);
    cprintf("PART 1 RESULT: %d ", min_dist_p1);
    if (min_dist_p1 == 14) cprintf("[PASS]\r\n"); else cprintf("[FAIL]\r\n");
    
    /* Part 2: Visit all points and return to 0 */
    solve_tsp_recursive(p_indices, 0, num_points - 2, 1);
    cprintf("PART 2 RESULT: %d ", min_dist_p2);
    if (min_dist_p2 == 20) cprintf("[PASS]\r\n"); else cprintf("[FAIL]\r\n");
}

int main(void) {
    /* Set up C64 screen colors */
    bgcolor(COLOR_BLACK);
    bordercolor(COLOR_GRAY3);
    textcolor(COLOR_YELLOW);
    clrscr();
    
    cprintf("ADVENT OF CODE 2016 - DAY 24\r\n");
    cprintf("AIR DUCT SPELUNKING\r\n");
    cprintf("============================\r\n\r\n");
    
    run_example();
    
    cprintf("\r\nDONE.\r\n");
    cprintf("PRESS ENTER TO EXIT.\r\n");
    cgetc();
    
    return 0;
}
