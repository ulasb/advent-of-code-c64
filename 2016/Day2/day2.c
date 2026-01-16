#include <stdio.h>
#include <string.h>
#include <conio.h>

/* Advent of Code 2016 - Day 2: Bathroom Security */

static const char* input[] = {
    "ULLRRDDRULURDULRLRUDLURDLUURDLRDULDRULRDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLURD",
    "RRDDLLUURRDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLURDLUR",
    "LLUUUURRRDDRRDLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLU",
    "UUDDLRLRLRUDDLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRLUDRL",
    "LRUDLRUDLRUDLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRLURDLUDRL"
};

#define INPUT_LINES 5

static char pad1[3][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'}
};

static char pad2[5][5] = {
    {0,   0,   '1', 0,   0  },
    {0,   '2', '3', '4', 0  },
    {'5', '6', '7', '8', '9'},
    {0,   'A', 'B', 'C', 0  },
    {0,   0,   'D', 0,   0  }
};

void solve(const char* instructions[], int lines, int part2) {
    int x, y;
    int i;
    
    if (part2) {
        x = 0; y = 2; /* Starts at '5' in pad2 */
    } else {
        x = 1; y = 1; /* Starts at '5' in pad1 */
    }

    for (i = 0; i < lines; ++i) {
        const char* p = instructions[i];
        while (*p) {
            char move = *p++;
            int next_x = x, next_y = y;
            if (move == 'U' && next_y > 0) next_y--;
            else if (move == 'D' && next_y < (part2 ? 4 : 2)) next_y++;
            else if (move == 'L' && next_x > 0) next_x--;
            else if (move == 'R' && next_x < (part2 ? 4 : 2)) next_x++;
            
            if (part2) {
                if (pad2[next_y][next_x] != 0) {
                    x = next_x;
                    y = next_y;
                }
            } else {
                x = next_x;
                y = next_y;
            }
        }
        textcolor(COLOR_WHITE);
        cprintf("%c", part2 ? pad2[y][x] : pad1[y][x]);
    }
    cprintf("\r\n");
}

void run_tests() {
    const char* sample_input[] = {
        "ULL",
        "RRDDD",
        "LURDL",
        "UUUUD"
    };
    
    textcolor(COLOR_YELLOW);
    cprintf("RUNNING TESTS...\r\n");
    
    textcolor(COLOR_LIGHTBLUE);
    cprintf("Sample Part 1: ");
    solve(sample_input, 4, 0); /* Should be 1985 */
    
    textcolor(COLOR_LIGHTBLUE);
    cprintf("Sample Part 2: ");
    solve(sample_input, 4, 1); /* Should be 5DB3 */
    cprintf("\r\n");
}

int main() {
    bgcolor(COLOR_BLACK);
    bordercolor(COLOR_GRAY1);
    clrscr();
    
    textcolor(COLOR_CYAN);
    cprintf("========================================\r\n");
    cprintf("   ADVENT OF CODE 2016 - DAY 2 (C64)    \r\n");
    cprintf("        - BATHROOM SECURITY -           \r\n");
    cprintf("========================================\r\n\r\n");

    run_tests();

    textcolor(COLOR_YELLOW);
    cprintf("PUZZLE RESULTS:\r\n");
    
    textcolor(COLOR_LIGHTBLUE);
    cprintf("Part 1: ");
    solve(input, INPUT_LINES, 0);
    
    textcolor(COLOR_LIGHTBLUE);
    cprintf("Part 2: ");
    solve(input, INPUT_LINES, 1);

    textcolor(COLOR_GREEN);
    cprintf("\r\nDONE! PRESS ANY KEY TO EXIT.");
    cgetc();

    return 0;
}
