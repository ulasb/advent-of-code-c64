#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

/* Advent of Code 2016 - Day 1: No Time for a Taxicab */

#define MAX_VISITED 1024

typedef struct {
    int x;
    int y;
} Point;

static Point visited[MAX_VISITED];
static int visited_count = 0;

/* Directions: 0 = North, 1 = East, 2 = South, 3 = West */
static int dx[] = { 0, 1, 0, -1 };
static int dy[] = { 1, 0, -1, 0 };

int manhattan_distance(int x, int y) {
    return abs(x) + abs(y);
}

int is_visited(int x, int y) {
    int i;
    for (i = 0; i < visited_count; ++i) {
        if (visited[i].x == x && visited[i].y == y) {
            return 1;
        }
    }
    return 0;
}

void mark_visited(int x, int y) {
    if (visited_count < MAX_VISITED) {
        visited[visited_count].x = x;
        visited[visited_count].y = y;
        visited_count++;
    }
}

static char buffer[256];

void solve(const char* input) {
    static int x, y;
    static int facing;
    static char* token;
    static int hq_x, hq_y;
    static int hq_found;
    static char turn;
    static int steps;
    static int i;

    x = 0; y = 0;
    facing = 0; /* 0: North */
    hq_found = 0;

    visited_count = 0;
    mark_visited(0, 0);

    strcpy(buffer, input);
    token = strtok(buffer, ", ");
    
    textcolor(COLOR_YELLOW);
    printf("Input: %s\n", input);
    textcolor(COLOR_LIGHTBLUE);

    while (token != NULL) {
        turn = token[0];
        steps = atoi(token + 1);

        if (turn == 'L') {
            facing = (facing + 3) % 4;
        } else if (turn == 'R') {
            facing = (facing + 1) % 4;
        }

        for (i = 0; i < steps; ++i) {
            x += dx[facing];
            y += dy[facing];

            if (!hq_found) {
                if (is_visited(x, y)) {
                    hq_x = x;
                    hq_y = y;
                    hq_found = 1;
                } else {
                    mark_visited(x, y);
                }
            }
        }
        token = strtok(NULL, ", ");
    }

    printf("  Part 1: ");
    textcolor(COLOR_WHITE);
    printf("%d blocks\n", manhattan_distance(x, y));
    
    textcolor(COLOR_LIGHTBLUE);
    printf("  Part 2: ");
    textcolor(COLOR_WHITE);
    if (hq_found) {
        printf("%d blocks\n", manhattan_distance(hq_x, hq_y));
    } else {
        printf("Not found\n");
    }
    printf("\n");
}

int main() {
    clrscr();
    textcolor(COLOR_CYAN);
    printf("========================================\n");
    printf("   ADVENT OF CODE 2016 - DAY 1 (C64)    \n");
    printf("      - NO TIME FOR A TAXICAB -         \n");
    printf("========================================\n\n");

    /* Sample 1 */
    solve("R2, L3");
    
    /* Sample 2 */
    solve("R2, R2, R2");

    /* Sample 3 */
    solve("R5, L5, R5, R3");

    /* HQ Sample for Part 2 */
    solve("R8, R4, R4, R8");

    printf("\nPress any key to exit...");
    cgetc();

    return 0;
}
