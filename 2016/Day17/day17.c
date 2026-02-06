#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

#define MAX_X 3
#define MAX_Y 3
#define MAX_PATH_LEN 1000

/* Direction mapping: 0: Up, 1: Down, 2: Left, 3: Right */
const char DIRS[] = "UDLR";
const int DX[] = {0, 0, -1, 1};
const int DY[] = {-1, 1, 0, 0};

/* Global buffers to save stack space */
char current_passcode[32];
uint8 digest[16];
char hex_hash[33];
char best_path[MAX_PATH_LEN];
int max_path_len = 0;
char next_path[128];
char path_buf[MAX_PATH_LEN + 1];

MD5_CTX passcode_ctx;
char last_passcode[32] = "";

void get_hex_hash(const char *passcode, const char *path) {
    MD5_CTX ctx;
    int path_len = strlen(path);
    static const char hex_chars[] = "0123456789abcdef";
    
    if (strcmp(last_passcode, passcode) != 0) {
        md5_init(&passcode_ctx);
        md5_update(&passcode_ctx, (uint8*)passcode, strlen(passcode));
        strcpy(last_passcode, passcode);
    }
    
    /* Clone the context after passcode */
    memcpy(&ctx, &passcode_ctx, sizeof(MD5_CTX));
    
    if (path_len > 0) {
        md5_update(&ctx, (uint8*)path, path_len);
    }
    md5_final(digest, &ctx);
    
    /* Only need first 4 characters */
    hex_hash[0] = hex_chars[digest[0] >> 4];
    hex_hash[1] = hex_chars[digest[0] & 0x0f];
    hex_hash[2] = hex_chars[digest[1] >> 4];
    hex_hash[3] = hex_chars[digest[1] & 0x0f];
    hex_hash[4] = '\0';
}

typedef struct {
    int x, y;
    int dir_index;
    char h[5];
} DFSState;

DFSState dfs_stack[MAX_PATH_LEN];

void find_longest(int start_x, int start_y) {
    int x, y, len, di;
    int top = 0;

    max_path_len = 0;
    
    dfs_stack[top].x = start_x;
    dfs_stack[top].y = start_y;
    dfs_stack[top].dir_index = 0;
    
    path_buf[0] = '\0';
    get_hex_hash(current_passcode, path_buf);
    memcpy(dfs_stack[top].h, hex_hash, 4);
    dfs_stack[top].h[4] = '\0';

    while (top >= 0) {
        x = dfs_stack[top].x;
        y = dfs_stack[top].y;
        di = dfs_stack[top].dir_index;
        len = top;

        if (x == MAX_X && y == MAX_Y) {
            if (len > max_path_len) {
                max_path_len = len;
            }
            top--;
            continue;
        }

        if (di < 4) {
            dfs_stack[top].dir_index++;
            if (dfs_stack[top].h[di] >= 'b' && dfs_stack[top].h[di] <= 'f') {
                int nx = x + DX[di];
                int ny = y + DY[di];
                if (nx >= 0 && nx <= MAX_X && ny >= 0 && ny <= MAX_Y) {
                    top++;
                    if (top >= MAX_PATH_LEN) {
                        top--; /* Should not happen with MAX_PATH_LEN 1000 */
                    } else {
                        dfs_stack[top].x = nx;
                        dfs_stack[top].y = ny;
                        dfs_stack[top].dir_index = 0;
                        path_buf[len] = DIRS[di];
                        path_buf[len + 1] = '\0';
                        get_hex_hash(current_passcode, path_buf);
                        memcpy(dfs_stack[top].h, hex_hash, 4);
                        dfs_stack[top].h[4] = '\0';
                    }
                }
            }
        } else {
            top--;
            if (top >= 0) {
                path_buf[top] = '\0';
            }
        }
    }
}

/* BFS for shortest path */
typedef struct {
    int x, y;
    int path_len;
    char path[128]; /* Shortest path probably not that long */
} State;

#define QUEUE_SIZE 100
State queue[QUEUE_SIZE];
int head = 0;
int tail = 0;

void enqueue(int x, int y, int len, const char *path) {
    if ((tail + 1) % QUEUE_SIZE == head) {
        /* Queue full - in a real BFS we'd handle this, but for shortest path 
           on C64 we hope it fits or we use a larger/different queue. */
        return;
    }
    queue[tail].x = x;
    queue[tail].y = y;
    queue[tail].path_len = len;
    strcpy(queue[tail].path, path);
    tail = (tail + 1) % QUEUE_SIZE;
}

State* dequeue() {
    State *s;
    if (head == tail) return NULL;
    s = &queue[head];
    head = (head + 1) % QUEUE_SIZE;
    return s;
}


char* find_shortest(const char *passcode) {
    int x, y, len, i;
    char path[128];
    char h[5];
    State *s;
    static char result[128];

    head = tail = 0;
    enqueue(0, 0, 0, "");

    while (head != tail) {
        s = dequeue();
        x = s->x;
        y = s->y;
        len = s->path_len;

        strcpy(path, s->path);

        if (x == MAX_X && y == MAX_Y) {
            strcpy(result, path);
            return result;
        }

        get_hex_hash(passcode, path);
        memcpy(h, hex_hash, 4);
        
        for (i = 0; i < 4; ++i) {
            char ch = h[i];
            if (ch >= 'b' && ch <= 'f') {
                int nx = x + DX[i];
                int ny = y + DY[i];
                if (nx >= 0 && nx <= MAX_X && ny >= 0 && ny <= MAX_Y) {
                    strcpy(next_path, path);
                    next_path[len] = DIRS[i];
                    next_path[len + 1] = '\0';
                    enqueue(nx, ny, len + 1, next_path);
                }
            }
        }
    }
    return NULL;
}

void run_tests() {
    const char *test_passcodes[] = {"ihgpwlah", "kglvqrro", "ulqzkmiv"};
    const char *expected_shortest[] = {"DDRRRD", "DDUDRLRRUDRD", "DRURDRUDDLLDLUURRDULRLDUUDDDRR"};
    int i;
    char *res;

    cprintf("RUNNING TESTS...\r\n");
    for (i = 0; i < 3; ++i) {
        res = find_shortest(test_passcodes[i]);
        cprintf("TEST %d: ", i + 1);
        if (res && strcmp(res, expected_shortest[i]) == 0) {
            cprintf("PASS\r\n");
        } else {
            cprintf("FAIL\r\nGOT: %s\r\n", res ? res : "NONE");
        }
    }
}


int main(void) {
    char *shortest;
    bgcolor(COLOR_BLUE);
    bordercolor(COLOR_LIGHTBLUE);
    textcolor(COLOR_WHITE);
    clrscr();

    cprintf("ADVENT OF CODE 2016 - DAY 17\r\n");
    cprintf("TWO STEPS FORWARD...\r\n");
    cprintf("============================\r\n");

    run_tests();

    cprintf("\r\nSOLVING FOR pslxynzg...\r\n");
    strcpy(current_passcode, "pslxynzg");
    
    shortest = find_shortest(current_passcode);
    if (shortest) {
        cprintf("SHORTEST: %s\r\n", shortest);
    } else {
        cprintf("SHORTEST: NOT FOUND\r\n");
    }

    cprintf("CALCULATING LONGEST...\r\n");
    find_longest(0, 0);
    cprintf("LONGEST LEN: %d\r\n", max_path_len);

    cprintf("\r\nPRESS ANY KEY TO EXIT.\r\n");
    cgetc();
    return 0;
}
