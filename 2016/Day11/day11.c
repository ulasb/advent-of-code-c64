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
#define QUEUE_SIZE 2500
#define BITSET_SIZE 7752

// Combinations table choose[n][k]
unsigned int choose[21][6];

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

typedef struct {
    unsigned char elevator;
    unsigned char pairs[MAX_PAIRS]; // Each byte: (G_floor << 2) | C_floor
} State;

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

unsigned char visited[BITSET_SIZE];

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
        if (s->pairs[i] != 0xFF) return 0; // (3 << 2) | 3 = 11 | 11 = 15 = 0x0F? No.
    }
    // Floor 3 is encoded as 3. (G << 2) | C = (3 << 2) | 3 = 12 + 3 = 15.
    // So for victory, all pairs must be 15.
    for (i = 0; i < num_pairs; ++i) {
        if (s->pairs[i] != 15) return 0;
    }
    return 1;
}

// Global queue to avoid stack pressure
State queue[QUEUE_SIZE];

int solve(State *initial, int num_pairs) {
    int head = 0, tail = 0;
    int dist = 0;
    int i, j, dir, p;
    unsigned int level_count;
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
    
    // For cases with fewer than 5 pairs, we pad the remaining pairs with a "dummy" floor
    // but the combination index assumes 5 items. 
    for (i = num_pairs; i < MAX_PAIRS; ++i) {
        initial->pairs[i] = 15; // Put on top floor so they don't move
    }

    mark_visited(get_state_id(initial));
    queue[tail++] = *initial;
    
    while (head < tail) {
        level_count = tail - head;
        while (level_count--) {
            curr = queue[head++];
            num_items = 0;
            
            if (is_victory(&curr, num_pairs)) return dist;
            
            // Find items on current floor
            for (p = 0; p < num_pairs; ++p) {
                if ((curr.pairs[p] >> 2) == curr.elevator) items[num_items++] = p * 2;     // Generator
                if ((curr.pairs[p] & 3) == curr.elevator) items[num_items++] = p * 2 + 1; // Chip
            }
            
            // Possible moves: 1 or 2 items
            for (i = 0; i < num_items; ++i) {
                for (j = i; j < num_items; ++j) {
                    for (dir = 1; dir >= -1; dir -= 2) {
                        next_e = (int)curr.elevator + dir;
                        
                        if (next_e < 0 || next_e >= NUM_FLOORS) continue;
                        
                        // Pruning: Don't move items down if all floors below are empty
                        if (dir == -1) {
                            empty_below = 1;
                            for (f_below = 0; f_below < curr.elevator; ++f_below) {
                                for (p = 0; p < num_pairs; ++p) {
                                    if ((curr.pairs[p] >> 2) == f_below || (curr.pairs[p] & 3) == f_below) {
                                        empty_below = 0;
                                        break;
                                    }
                                }
                                if (!empty_below) break;
                            }
                            if (empty_below) continue;
                        }
                        
                        next = curr;
                        next.elevator = (unsigned char)next_e;
                        
                        // Move item i
                        if (items[i] % 2 == 0) next.pairs[items[i] / 2] = (next.pairs[items[i] / 2] & 3) | (next.elevator << 2);
                        else next.pairs[items[i] / 2] = (next.pairs[items[i] / 2] & 0xFC) | next.elevator;
                        
                        // Move item j (if different)
                        if (j != i) {
                            if (items[j] % 2 == 0) next.pairs[items[j] / 2] = (next.pairs[items[j] / 2] & 3) | (next.elevator << 2);
                            else next.pairs[items[j] / 2] = (next.pairs[items[j] / 2] & 0xFC) | next.elevator;
                        }
                        
                        if (is_valid(&next)) {
                            next_id = get_state_id(&next);
                            if (!is_visited(next_id)) {
                                if (tail >= QUEUE_SIZE) {
                                    // Queue overflow!
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
        dist++;
    }
    
    return -1;
}

void run_test_example() {
    State s;
    int result;
    s.elevator = 0;
    // Example: H-chip (C0), L-chip (C0), H-gen (G1), L-gen (G2)
    // H (G1, C0) -> 01 00 -> 4
    // L (G2, C0) -> 10 00 -> 8
    s.pairs[0] = 4;
    s.pairs[1] = 8;
    
    cprintf("RUNNING EXAMPLE TEST...\r\n");
    result = solve(&s, 2);
    if (result == 11) {
        textcolor(COLOR_GREEN);
        cprintf("EXAMPLE TEST PASSED: 11 STEPS\r\n");
    } else {
        textcolor(COLOR_RED);
        cprintf("EXAMPLE TEST FAILED: GOT %d, EXPECTED 11\r\n", result);
    }
    textcolor(COLOR_WHITE);
}

void run_part1() {
    State s;
    int result;
    s.elevator = 0;
    /* User Input:
     * strontium: G0, C0 (0)
     * plutonium: G0, C0 (0)
     * thulium: G1, C2 (6)
     * ruthenium: G1, C1 (5)
     * curium: G1, C1 (5)
     */
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
    } else if (result == -2) {
        textcolor(COLOR_RED);
        cprintf("PART 1 FAILED: QUEUE OVERFLOW\r\n");
    } else {
        textcolor(COLOR_RED);
        cprintf("PART 1 FAILED: NO SOLUTION\r\n");
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
