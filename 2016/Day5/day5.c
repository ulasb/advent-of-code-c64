#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

static const char HEX_DIGITS[] = "0123456789abcdef";

/* Global contexts to save stack space */
MD5_CTX global_ctx;
char password[9];
char id_with_index[32];

/* Function Prototypes */
void run_tests(void);
void solve_part1(const char *door_id);
void solve_part2(const char *door_id);

int main(void) {
    bgcolor(COLOR_BLUE);
    bordercolor(COLOR_LIGHTBLUE);
    textcolor(COLOR_WHITE);
    clrscr();

    cprintf("ADVENT OF CODE 2016 - DAY 5\r\n");
    cprintf("HOW ABOUT A NICE GAME OF CHESS?\r\n");
    cprintf("==============================\r\n");

    run_tests();

    /* Actual puzzle input would go here, but we use the test cases */
    /* solve_part1("cxdnnyjw"); */

    cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
    cgetc();

    return 0;
}

void run_tests(void) {
    cprintf("\r\nRUNNING TESTS...\r\n");
    cprintf("TEST PART 1 (ID: abc):\r\n");
    cprintf("NOTE: MD5 is extremely slow on C64.\r\n");
    cprintf("Searching for first character...\r\n");
    
    solve_part1("abc");
    
    cprintf("\r\nTEST PART 2 (ID: abc):\r\n");
    /* solve_part2("abc"); */
    cprintf("Logic for Part 2 is implemented in\r\n");
    cprintf("code but skipped for this demo.\r\n");
}

void solve_part1(const char *door_id) {
    uint32 index = 0;
    int found = 0;
    uint8 digest[16];
    int id_len = strlen(door_id);
    
    memset(password, 0, sizeof(password));
    memcpy(id_with_index, door_id, id_len);
    
    while (found < 8) {
        /* Construct input string: door_id + index */
        ltoa(index, id_with_index + id_len, 10);
        
        /* MD5 Hash */
        md5_init(&global_ctx);
        md5_update(&global_ctx, (const uint8*)id_with_index, strlen(id_with_index));
        md5_final(digest, &global_ctx);
        
        /* Check for 5 leading zeros (in hex) */
        if (digest[0] == 0 && digest[1] == 0 && digest[2] < 16) {
            password[found] = HEX_DIGITS[digest[2] & 0x0F];
            cprintf("\r\nFound char %d: %c (idx: %lu)  \r\n", found + 1, password[found], index);
            found++;
        }
        
        index++;
        
        /* Progress indicator */
        if ((index & 0x1FF) == 0) {
            cprintf("\rHashing: %lu", index);
            if (kbhit()) {
                if (cgetc() == 27) break; /* ESC to stop */
            }
        }
        
        /* Demo limit */
        if (found == 1) {
            cprintf("\r\nDemo: Found 1st char and stopped.\r\n");
            break;
        }
    }
}

void solve_part2(const char *door_id) {
    uint32 index = 0;
    int found_count = 0;
    uint8 digest[16];
    uint8 pos;
    char char_val;
    int id_len = strlen(door_id);
    
    memset(password, '_', 8);
    password[8] = 0;
    memcpy(id_with_index, door_id, id_len);
    
    while (found_count < 8) {
        /* Construct input string: door_id + index */
        ltoa(index, id_with_index + id_len, 10);
        
        md5_init(&global_ctx);
        md5_update(&global_ctx, (const uint8*)id_with_index, strlen(id_with_index));
        md5_final(digest, &global_ctx);
        
        if (digest[0] == 0 && digest[1] == 0 && digest[2] < 16) {
            pos = digest[2] & 0x0F;
            if (pos < 8 && password[pos] == '_') {
                char_val = HEX_DIGITS[digest[3] >> 4];
                password[pos] = char_val;
                found_count++;
                cprintf("\r\nPos %d found: %c (idx: %lu)  \r\n", pos, char_val, index);
                cprintf("Password: %s\r\n", password);
            }
        }
        
        index++;
        if ((index & 0x1FF) == 0) {
            cprintf("\rHashing: %lu", index);
            if (kbhit()) if (cgetc() == 27) break;
        }
    }
}
