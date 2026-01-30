#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Constants */
#define MAP_SIZE 64
#define VISITED_SIZE (MAP_SIZE * MAP_SIZE / 8)
#define QUEUE_SIZE (MAP_SIZE * MAP_SIZE)
#define ERR_QUEUE_OVERFLOW -2

/* Global buffers to stay off the stack */
unsigned char visited[VISITED_SIZE];
unsigned char qx[QUEUE_SIZE];
unsigned char qy[QUEUE_SIZE];
unsigned int qhead, qtail;

/* Function Prototypes */
void run_tests(void);
int solve_part1(unsigned char start_x, unsigned char start_y, 
                unsigned char target_x, unsigned char target_y, 
                unsigned int favorite_number);
int solve_part2(unsigned char start_x, unsigned char start_y, 
                unsigned char max_steps, unsigned int favorite_number);
unsigned char is_wall(unsigned char x, unsigned char y, unsigned int favorite_number);
void set_visited(unsigned char x, unsigned char y);
unsigned char get_visited(unsigned char x, unsigned char y);

int main(void) {
    bgcolor(COLOR_BLUE);
    bordercolor(COLOR_LIGHTBLUE);
    textcolor(COLOR_WHITE);
    clrscr();
    gotoxy(0, 0);

    cprintf("ADVENT OF CODE 2016 - DAY 13\r\n");
    cprintf("MAZE OF TWISTY LITTLE CUBICLES\r\n");
    cprintf("==============================\r\n");

    run_tests();

    cprintf("\r\n--- FINAL ANSWERS ---\r\n");
    /* Part 1: Start (1,1), Target (31,39), Favorite 1358 */
    cprintf("PART 1: %d\r\n", solve_part1(1, 1, 31, 39, 1358));
    /* Part 2: Start (1,1), Max 50 steps, Favorite 1358 */
    cprintf("PART 2: %d\r\n", solve_part2(1, 1, 50, 1358));

    cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
    cgetc();

    return 0;
}

unsigned char is_wall(unsigned char x, unsigned char y, unsigned int favorite_number) {
    unsigned int val;
    unsigned char bits = 0;
    
    /* Using the simplified formula: (x+y)^2 + 3x + y + favorite_number */
    /* Wait, checking the original: x*x + 3*x + 2*x*y + y + y*y + FAVORITE_NUMBER */
    /* x*x + 2*x*y + y*y + 3*x + y + FAVORITE_NUMBER */
    /* (x+y)*(x+y) + 3*x + y + FAVORITE_NUMBER */
    unsigned int xpyp2;
    unsigned int x_u = x;
    unsigned int y_u = y;
    
    xpyp2 = (x_u + y_u) * (x_u + y_u);
    val = xpyp2 + 3 * x_u + y_u + favorite_number;

    while (val > 0) {
        if (val & 1) bits++;
        val >>= 1;
    }
    return bits % 2;
}

void set_visited(unsigned char x, unsigned char y) {
    unsigned int pos = (unsigned int)y * MAP_SIZE + x;
    visited[pos >> 3] |= (1 << (pos & 7));
}

unsigned char get_visited(unsigned char x, unsigned char y) {
    unsigned int pos = (unsigned int)y * MAP_SIZE + x;
    return (visited[pos >> 3] & (1 << (pos & 7))) != 0;
}

int solve_part1(unsigned char start_x, unsigned char start_y, 
                unsigned char target_x, unsigned char target_y, 
                unsigned int favorite_number) {
    unsigned char x, y, nx, ny;
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    int i;
    unsigned int steps = 0;
    unsigned int current_level_size;
    unsigned int next_level_size;

    memset(visited, 0, sizeof(visited));
    qhead = 0;
    qtail = 0;

    qx[qtail] = start_x;
    qy[qtail] = start_y;
    qtail = (qtail + 1) % QUEUE_SIZE;
    set_visited(start_x, start_y);
    current_level_size = 1;

    while (qhead != qtail) {
        next_level_size = 0;
        while (current_level_size > 0) {
            x = qx[qhead];
            y = qy[qhead];
            qhead = (qhead + 1) % QUEUE_SIZE;
            current_level_size--;

            if (x == target_x && y == target_y) return steps;

            for (i = 0; i < 4; i++) {
                nx = x + dx[i];
                ny = y + dy[i];

                /* Check bounds (x,y >= 0 is implicit since nx,ny are unsigned char) */
                /* But we need to handle underflow from x-1 or y-1 */
                if (dx[i] == -1 && x == 0) continue;
                if (dy[i] == -1 && y == 0) continue;
                if (nx >= MAP_SIZE || ny >= MAP_SIZE) continue;

                if (!get_visited(nx, ny) && !is_wall(nx, ny, favorite_number)) {
                    set_visited(nx, ny);
                    qx[qtail] = nx;
                    qy[qtail] = ny;
                    qtail = (qtail + 1) % QUEUE_SIZE;
                    next_level_size++;
                    
                    /* Queue overflow check */
                    if (qtail == qhead) return ERR_QUEUE_OVERFLOW; 
                }
            }
        }
        steps++;
        current_level_size = next_level_size;
    }

    return -1; /* Unreachable */
}

int solve_part2(unsigned char start_x, unsigned char start_y, 
                unsigned char max_steps, unsigned int favorite_number) {
    unsigned char x, y, nx, ny;
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    int i;
    unsigned int steps = 0;
    unsigned int current_level_size;
    unsigned int next_level_size;
    unsigned int total_visited = 0;

    memset(visited, 0, sizeof(visited));
    qhead = 0;
    qtail = 0;

    qx[qtail] = start_x;
    qy[qtail] = start_y;
    qtail = (qtail + 1) % QUEUE_SIZE;
    set_visited(start_x, start_y);
    current_level_size = 1;
    total_visited = 1;

    while (qhead != qtail && steps < max_steps) {
        next_level_size = 0;
        while (current_level_size > 0) {
            x = qx[qhead];
            y = qy[qhead];
            qhead = (qhead + 1) % QUEUE_SIZE;
            current_level_size--;

            for (i = 0; i < 4; i++) {
                nx = x + dx[i];
                ny = y + dy[i];

                if (dx[i] == -1 && x == 0) continue;
                if (dy[i] == -1 && y == 0) continue;
                if (nx >= MAP_SIZE || ny >= MAP_SIZE) continue;

                if (!get_visited(nx, ny) && !is_wall(nx, ny, favorite_number)) {
                    set_visited(nx, ny);
                    qx[qtail] = nx;
                    qy[qtail] = ny;
                    qtail = (qtail + 1) % QUEUE_SIZE;
                    next_level_size++;
                    total_visited++;
                    
                    if (qtail == qhead) return ERR_QUEUE_OVERFLOW;
                }
            }
        }
        steps++;
        current_level_size = next_level_size;
    }

    return total_visited;
}

void run_tests(void) {
    int res;
    cprintf("\r\nRUNNING TESTS...\r\n");

    /* Example from AoC: Favorite 10, Target (7, 4) should be 11 steps */
    cprintf("TEST PART 1 (7,4 @ 10): ");
    res = solve_part1(1, 1, 7, 4, 10);
    if (res == 11)
        cprintf("PASS\r\n");
    else
        cprintf("FAIL (got %d)\r\n", res);
}
