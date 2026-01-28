#include <stdio.h>
#include <string.h>
#include <conio.h>

/* 
 * Advent of Code 2016 Day 11 - C64 version
 * Ported from Python implementation with symmetry breaking.
 * 
 * Symmetry breaking uses a canonical state where pairs are sorted.
 * We map the sorted pairs to a combination index using the combinatorial number system.
 */

#define MAX_PAIRS 5
#define NUM_FLOORS 4
#define QUEUE_SIZE 4000
#define BITSET_SIZE 7752

// Combinations table choose[n][k]
unsigned int choose[21][6];

typedef struct {
    unsigned char elevator;
    unsigned char pairs[MAX_PAIRS]; // Each byte: (G_floor << 2) | C_floor
} State;

State queue[QUEUE_SIZE];
unsigned char visited[BITSET_SIZE];

void init_choose() {
    int n, k;
    for (n = 0; n <= 20; ++n) {
        choose[n][0] = 1;
        for (k = 1; k <= 5; ++k) {
            if (k > n) choose[n][k] = 0;
            else if (k == n) choose[n][k] = 1;
            else choose[n][k] = choose[n - 1][k - 1] + choose[n - 1][k];
        }
    }
}

// Pack state to 16-bit ID for the visited bitset
unsigned int get_state_id(State *s) {
    unsigned char sorted[MAX_PAIRS];
    int i, j;
    unsigned int id;
    
    // Copy and sort pairs
    memcpy(sorted, s->pairs, MAX_PAIRS);
    for (i = 0; i < MAX_PAIRS - 1; ++i) {
        for (j = i + 1; j < MAX_PAIRS; ++j) {
            if (sorted[i] > sorted[j]) {
                unsigned char tmp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = tmp;
            }
        }
    }
    
    // Combination index for 5 items from 16 types:
    // Index = choose(p1, 1) + choose(p2+1, 2) + choose(p3+2, 3) + choose(p4+3, 4) + choose(p5+4, 5)
    id = (unsigned int)s->elevator * 15504;
    id += choose[sorted[0]][1];
    id += choose[sorted[1] + 1][2];
    id += choose[sorted[2] + 2][3];
    id += choose[sorted[3] + 3][4];
    id += choose[sorted[4] + 4][5];
    return id;
}

void mark_visited(unsigned int id) {
    visited[id >> 3] |= (1 << (id & 7));
}

int is_visited(unsigned int id) {
    return visited[id >> 3] & (1 << (id & 7));
}

int is_valid(State *s) {
    int f, p;
    for (f = 0; f < NUM_FLOORS; ++f) {
        int has_gen = 0;
        for (p = 0; p < MAX_PAIRS; ++p) {
            if ((s->pairs[p] >> 2) == f) {
                has_gen = 1;
                break;
            }
        }
        if (!has_gen) continue;
        
        for (p = 0; p < MAX_PAIRS; ++p) {
            // If chip is on this floor...
            if ((s->pairs[p] & 3) == f) {
                // ...and its generator is NOT on this floor
                if ((s->pairs[p] >> 2) != f) return 0;
            }
        }
    }
    return 1;
}

int is_victory(State *s, int num_pairs) {
    int i;
    for (i = 0; i < num_pairs; ++i) {
        if (s->pairs[i] != 15) return 0;
    }
    return 1;
}

int solve(State *initial, int num_pairs) {
    int head = 0, tail = 0;
    int dist = 0;
    int i, j, dir, p;
    unsigned int level_count;
    unsigned char prog_y;
    State curr;
    State next;
    unsigned char items[10];
    int num_items;
    int next_e;
    unsigned int next_id;
    int empty_below;
    int f_below;
    
    // Reset visited bitset
    memset(visited, 0, BITSET_SIZE);
    
    // Pad remaining pairs
    for (i = num_pairs; i < MAX_PAIRS; ++i) {
        initial->pairs[i] = 15;
    }

    mark_visited(get_state_id(initial));
    queue[tail++] = *initial;
    
    prog_y = wherey();
    while (head < tail) {
        level_count = tail - head;
        gotoxy(0, prog_y);
        cclear(40);
        gotoxy(0, prog_y);
        cprintf("Depth %d: %u states", dist, level_count);
        
        while (level_count--) {
            curr = queue[head++];
            num_items = 0;
            
            if (is_victory(&curr, num_pairs)) {
                cprintf("\r\n");
                return dist;
            }
            
            for (p = 0; p < num_pairs; ++p) {
                if ((curr.pairs[p] >> 2) == curr.elevator) items[num_items++] = p * 2;
                if ((curr.pairs[p] & 3) == curr.elevator) items[num_items++] = p * 2 + 1;
            }
            
            for (i = 0; i < num_items; ++i) {
                for (j = i; j < num_items; ++j) {
                    for (dir = 1; dir >= -1; dir -= 2) {
                        next_e = (int)curr.elevator + dir;
                        if (next_e < 0 || next_e >= NUM_FLOORS) continue;
                        
                        if (dir == -1) {
                            empty_below = 1;
                            for (f_below = 0; f_below < curr.elevator; ++f_below) {
                                for (p = 0; p < num_pairs; ++p) {
                                    if ((curr.pairs[p] >> 2) == f_below || (curr.pairs[p] & 3) == f_below) {
                                        empty_below = 0; break;
                                    }
                                }
                                if (!empty_below) break;
                            }
                            if (empty_below) continue;
                        }
                        
                        next = curr;
                        next.elevator = (unsigned char)next_e;
                        if (items[i] % 2 == 0) next.pairs[items[i] / 2] = (next.pairs[items[i] / 2] & 3) | (next.elevator << 2);
                        else next.pairs[items[i] / 2] = (next.pairs[items[i] / 2] & 0xFC) | next.elevator;
                        
                        if (j != i) {
                            if (items[j] % 2 == 0) next.pairs[items[j] / 2] = (next.pairs[items[j] / 2] & 3) | (next.elevator << 2);
                            else next.pairs[items[j] / 2] = (next.pairs[items[j] / 2] & 0xFC) | next.elevator;
                        }
                        
                        if (is_valid(&next)) {
                            next_id = get_state_id(&next);
                            if (!is_visited(next_id)) {
                                if (tail >= QUEUE_SIZE) {
                                    cprintf("\r\nQUEUE OVERFLOW!\r\n");
                                    return -2;
                                }
                                mark_visited(next_id);
                                queue[tail++] = next;
                            }
                        }
                    }
                }
            }
        }
        
        if (head > 0) {
            unsigned int remaining = tail - head;
            if (remaining > 0) memmove(queue, &queue[head], remaining * sizeof(State));
            head = 0;
            tail = remaining;
        }
        dist++;
    }
    cprintf("\r\n");
    return -1;
}

void run_test_example() {
    State s;
    int result;
    s.elevator = 0;
    s.pairs[0] = 4;
    s.pairs[1] = 8;
    cprintf("RUNNING EXAMPLE TEST...\r\n");
    result = solve(&s, 2);
    if (result == 11) {
        textcolor(COLOR_GREEN);
        cprintf("EXAMPLE TEST PASSED: 11 STEPS\r\n");
    } else {
        textcolor(COLOR_RED);
        cprintf("EXAMPLE TEST FAILED: GOT %d\r\n", result);
    }
    textcolor(COLOR_WHITE);
}

void run_part1() {
    State s;
    int result;
    s.elevator = 0;
    s.pairs[0] = 0;
    s.pairs[1] = 0;
    s.pairs[2] = 6;
    s.pairs[3] = 5;
    s.pairs[4] = 5;
    cprintf("SOLVING PART 1...\r\n");
    result = solve(&s, 5);
    if (result >= 0) {
        textcolor(COLOR_GREEN);
        cprintf("PART 1 SUCCESS: %d STEPS\r\n", result);
    } else {
        textcolor(COLOR_RED);
        cprintf("PART 1 FAILED: %s\r\n", (result == -2 ? "OVERFLOW" : "NO SOL"));
    }
    textcolor(COLOR_WHITE);
}

int main() {
    clrscr();
    bgcolor(COLOR_BLUE);
    textcolor(COLOR_WHITE);
    cprintf("ADVENT OF CODE 2016 - DAY 11\r\n");
    cprintf("----------------------------\r\n");
    init_choose();
    run_test_example();
    cprintf("\r\n");
    run_part1();
    cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
    cgetc();
    return 0;
}
