#ifdef __CC65__
#include <conio.h>
#else
/* Host stubs for C64 conio functions when building for desktop testing */
#include <stdio.h>
#define cprintf printf
#define cgetc() getchar()
#define bgcolor(x) ((void)0)
#define bordercolor(x) ((void)0)
#define textcolor(x) ((void)0)
#define clrscr() ((void)0)
#define COLOR_BLUE 0
#define COLOR_LIGHTBLUE 1
#define COLOR_WHITE 2
#define COLOR_BLACK 3
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

#define MAX_X 3
#define MAX_Y 3
#define MAX_PATH_LEN 1000

/* Direction mapping: 0: Up, 1: Down, 2: Left, 3: Right */
const char DIRS[] = "UDLR";
/* Explicit ASCII byte values for direction letters (use these when
    constructing path strings so stored bytes are ASCII even under cc65). */
const char ASCII_DIRS[] = { 0x55, 0x44, 0x4C, 0x52, 0 };
const int DX[] = {0, 0, -1, 1};
const int DY[] = {-1, 1, 0, 0};

/* Global buffers to save stack space */
char current_passcode[32];
uint8_t digest[16];
char hex_hash[33];
char best_path[MAX_PATH_LEN];
int max_path_len = 0;
char next_path[128];
char path_buf[MAX_PATH_LEN + 1];

MD5_CTX passcode_ctx;
char last_passcode[32] = "";

uint8_t petscii_to_ascii(uint8_t c) {
    /* Map PETSCII lowercase letter range (0x41-0x5A) to ASCII lowercase (0x61-0x7A).
       Leave other bytes unchanged. */
    if (c >= 0x41 && c <= 0x5A) return c + 0x20;
    return c;
}

/* Host-side helper: map ASCII lowercase to PETSCII lowercase (inverse) */
uint8_t ascii_to_petscii(uint8_t c) {
    if (c >= 'a' && c <= 'z') return (uint8_t)(c - 0x20);
    return c;
}

static MD5_CTX static_ctx;
static uint8_t global_digest[16];

void get_hex_hash(const char *passcode, const char *path) {
    int path_len = strlen(path);
    int pass_len = strlen(passcode);
    static const char hex_chars[] = "0123456789abcdef";
    static uint8_t buf[256];
    int i;

            for (i = 0; i < pass_len; ++i) {
        #if defined(__CC65__) || defined(FORCE_PETSCII_TO_ASCII)
            buf[i] = petscii_to_ascii((uint8_t)passcode[i]);
        #elif defined(SIMULATE_PETSCII)
            buf[i] = ascii_to_petscii((uint8_t)passcode[i]);
        #else
            buf[i] = (uint8_t)passcode[i];
        #endif
            }
            /* Path characters are produced by the program using ASCII_DIRS, so
            always use their byte values directly (no PETSCII->ASCII conversion). */
            for (i = 0; i < path_len; ++i) {
             buf[pass_len + i] = (uint8_t)path[i];
            }

    MD5Init(&static_ctx);
    MD5Update(&static_ctx, buf, (uint32_t)(pass_len + path_len));
    MD5Final(global_digest, &static_ctx);
    
    (void)passcode;
    
    /* Only need first 4 characters of the hex hash */
    hex_hash[0] = hex_chars[global_digest[0] >> 4];
    hex_hash[1] = hex_chars[global_digest[0] & 0x0f];
    hex_hash[2] = hex_chars[global_digest[1] >> 4];
    hex_hash[3] = hex_chars[global_digest[1] & 0x0f];
    hex_hash[4] = '\0';
}

/* Convert a PETSCII path string in-place to ASCII (only for cc65 builds) */
static void petscii_str_to_ascii_inplace(char *s) {
#if defined(__CC65__)
    char *p = s;
    while (*p) {
        uint8_t c = (uint8_t)*p;
        uint8_t a = petscii_to_ascii(c);
        /* Convert to uppercase ASCII for direction letters so tests match expected uppercase output */
        if (a >= 'a' && a <= 'z') a = (uint8_t)(a - 0x20);
        *p = (char)a;
        p++;
    }
#else
    (void)s;
#endif
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
                        path_buf[len] = ASCII_DIRS[di];
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

#define QUEUE_SIZE 80
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
            petscii_str_to_ascii_inplace(result);
            return result;
        }

        get_hex_hash(passcode, path);
        memcpy(h, hex_hash, 4);
    (void)hex_hash; (void)path; (void)passcode; (void)i;
        
        for (i = 0; i < 4; ++i) {
            char ch = h[i];
            if (ch >= 'b' && ch <= 'f') {
                int nx = x + DX[i];
                int ny = y + DY[i];
                if (nx >= 0 && nx <= MAX_X && ny >= 0 && ny <= MAX_Y) {
                    strcpy(next_path, path);
                    next_path[len] = ASCII_DIRS[i];
                    next_path[len + 1] = '\0';
                    (void)next_path;
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
        /* Print first-4-hex chars for the empty-path hash to diagnose encoding */
        get_hex_hash(test_passcodes[i], "");
        cprintf("HASH[%s] = %s\r\n", test_passcodes[i], hex_hash);

        res = find_shortest(test_passcodes[i]);
        cprintf("TEST %d: ", i + 1);
        /* Ensure expected literal is converted to ASCII when running on cc65 */
        char expected_ascii[128];
        strcpy(expected_ascii, expected_shortest[i]);
#if defined(__CC65__) || defined(FORCE_PETSCII_TO_ASCII) || defined(SIMULATE_PETSCII)
        petscii_str_to_ascii_inplace(expected_ascii);
#endif
        if (res && strcmp(res, expected_ascii) == 0) {
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
#if defined(__CC65__)
    cprintf("BUILD: __CC65__ defined\r\n");
#elif defined(SIMULATE_PETSCII)
    cprintf("BUILD: SIMULATE_PETSCII (host simulation)\r\n");
#else
    cprintf("BUILD: host native build\r\n");
#endif
    /* show mapping sanity check */
    cprintf("MAPPING: petscii 0x41 -> ascii 0x%02x (%c)\r\n", (int)petscii_to_ascii(0x41), petscii_to_ascii(0x41));
    cprintf("TWO STEPS FORWARD...\r\n");
    cprintf("============================\r\n");

    run_tests();

    cprintf("\r\nSOLVING FOR pslxynzg...\r\n");
    strcpy(current_passcode, "pslxynzg");
    /* If building for the C64 (or forcing conversion), convert the passcode
       literal from PETSCII to ASCII once so subsequent hashing uses ASCII
       bytes. */
#if defined(__CC65__) || defined(FORCE_PETSCII_TO_ASCII)
    petscii_str_to_ascii_inplace(current_passcode);
#endif
#if defined(SIMULATE_PETSCII) && defined(FORCE_PETSCII_TO_ASCII)
    /* When simulating C64 literals on the host, convert the ASCII literal to
       PETSCII so that the code path which converts PETSCII->ASCII behaves
       like the real C64 build. */
    {
        int pi;
        for (pi = 0; current_passcode[pi]; ++pi) {
            current_passcode[pi] = (char)ascii_to_petscii((uint8_t)current_passcode[pi]);
        }
    }
#endif
    
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
