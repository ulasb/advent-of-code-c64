#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Constants */
#define MAX_BOTS 32
#define MAX_OUTPUTS 16
#define TYPE_BOT 0
#define TYPE_OUT 1

/* Bot structure */
typedef struct {
    int chips[2];
    int chip_count;
    char low_type;   /* 0 for Bot, 1 for Output */
    int low_id;
    char high_type;
    int high_id;
    char defined;    /* 1 if directions are set */
} Bot;

/* Global state to save stack space */
Bot bots[MAX_BOTS];
int outputs[MAX_OUTPUTS];
int ready_queue[MAX_BOTS];
int head, tail, q_count;

/* Function prototypes */
void init_system(void);
void add_chip(int bot_id, int value);
void set_directions(int bot_id, char l_type, int l_id, char h_type, int h_id);
void process_bots(int target_low, int target_high);
void run_test_case(void);

int main(void) {
    /* Set up screen */
    bgcolor(COLOR_BLACK);
    bordercolor(COLOR_GRAY3);
    textcolor(COLOR_YELLOW);
    clrscr();

    cprintf("ADVENT OF CODE 2016 - DAY 10\r\n");
    cprintf("BALANCE BOTS\r\n");
    cprintf("============================\r\n\r\n");

    run_test_case();

    cprintf("\r\nPRESS ANY KEY TO EXIT.\r\n");
    cgetc();

    return 0;
}

void init_system(void) {
    int i;
    memset(bots, 0, sizeof(bots));
    for (i = 0; i < MAX_OUTPUTS; i++) {
        outputs[i] = -1;
    }
    head = 0;
    tail = 0;
    q_count = 0;
}

void queue_bot(int bot_id) {
    if (q_count < MAX_BOTS) {
        ready_queue[tail] = bot_id;
        tail = (tail + 1) % MAX_BOTS;
        q_count++;
    }
}

void add_chip(int bot_id, int value) {
    if (bot_id >= MAX_BOTS) return;
    bots[bot_id].chips[bots[bot_id].chip_count++] = value;
    if (bots[bot_id].chip_count == 2 && bots[bot_id].defined) {
        queue_bot(bot_id);
    }
}

void set_directions(int bot_id, char l_type, int l_id, char h_type, int h_id) {
    if (bot_id >= MAX_BOTS) return;
    bots[bot_id].low_type = l_type;
    bots[bot_id].low_id = l_id;
    bots[bot_id].high_type = h_type;
    bots[bot_id].high_id = h_id;
    bots[bot_id].defined = 1;
    
    /* If it already has 2 chips, it can now be queued */
    if (bots[bot_id].chip_count == 2) {
        queue_bot(bot_id);
    }
}

void process_bots(int target_low, int target_high) {
    while (q_count > 0) {
        int bot_id = ready_queue[head];
        Bot *b = &bots[bot_id];
        int low, high;

        head = (head + 1) % MAX_BOTS;
        q_count--;

        if (b->chips[0] < b->chips[1]) {
            low = b->chips[0];
            high = b->chips[1];
        } else {
            low = b->chips[1];
            high = b->chips[0];
        }

        /* Part 1 check */
        if (low == target_low && high == target_high) {
            textcolor(COLOR_WHITE);
            cprintf("PART 1: BOT %d COMPARES %d & %d\r\n", bot_id, low, high);
            textcolor(COLOR_YELLOW);
        }

        /* Distribute low */
        if (b->low_type == TYPE_BOT) {
            add_chip(b->low_id, low);
        } else {
            if (b->low_id < MAX_OUTPUTS) outputs[b->low_id] = low;
        }

        /* Distribute high */
        if (b->high_type == TYPE_BOT) {
            add_chip(b->high_id, high);
        } else {
            if (b->high_id < MAX_OUTPUTS) outputs[b->high_id] = high;
        }

        b->chip_count = 0;
    }
}

void run_test_case(void) {
    long product;
    init_system();

    cprintf("RUNNING SAMPLE TEST...\r\n");
    cprintf("----------------------------\r\n");

    /* Sample instructions from AoC 2016 Day 10 */
    set_directions(2, TYPE_BOT, 1, TYPE_BOT, 0);
    set_directions(1, TYPE_OUT, 1, TYPE_BOT, 0);
    set_directions(0, TYPE_OUT, 2, TYPE_OUT, 0);

    add_chip(2, 5);
    add_chip(1, 3);
    add_chip(2, 2);

    /* For test case, look for comparison of 2 and 5 */
    process_bots(2, 5);

    cprintf("\r\nRESULTS:\r\n");
    cprintf("BINS (0,1,2): [%d, %d, %d]\r\n", outputs[0], outputs[1], outputs[2]);
    
    if (outputs[0] != -1 && outputs[1] != -1 && outputs[2] != -1) {
        product = (long)outputs[0] * outputs[1] * outputs[2];
        cprintf("PRODUCT: %ld\r\n", product);
        
        if (product == 30) {
            textcolor(COLOR_GREEN);
            cprintf(">> STATUS: PASS\r\n");
        } else {
            textcolor(COLOR_RED);
            cprintf(">> STATUS: FAIL (Expected 30)\r\n");
        }
    } else {
        textcolor(COLOR_RED);
        cprintf(">> STATUS: FAIL (Outputs missing)\r\n");
    }
    textcolor(COLOR_YELLOW);
}
