#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

/* Constants for memory management */
#define MAX_ROOM_LEN 80
#define MAX_CHECKSUM_LEN 6
#define MAX_FREQ 26

/* Freq structure for sorting */
typedef struct {
    char letter;
    int count;
} Freq;

/* Custom comparison function for sorting letter frequencies */
int compare_freq(const void* a, const void* b) {
    Freq* fa = (Freq*)a;
    Freq* fb = (Freq*)b;
    if (fa->count != fb->count) {
        return fb->count - fa->count; /* Descending by frequency */
    }
    return fa->letter - fb->letter; /* Ascending alphabetically for ties */
}

/* Validate room and extract sector_id */
int validate_room(const char* room, int* sector_id) {
    Freq freqs[26];
    char checksum[MAX_CHECKSUM_LEN];
    const char *p_dash, *p_bracket;
    int last_dash_idx;
    int i;
    
    /* Initialize frequency table */
    for (i = 0; i < 26; i++) {
        freqs[i].letter = 'a' + i;
        freqs[i].count = 0;
    }
    
    /* Find markers in the room string efficiently */
    p_dash = strrchr(room, '-');
    p_bracket = strchr(room, '[');
    
    if (p_dash == NULL || p_bracket == NULL) return 0;
    
    last_dash_idx = (int)(p_dash - room);
    
    /* Extract Sector ID */
    *sector_id = atoi(p_dash + 1);
    
    /* Extract Checksum */
    if (!strchr(p_bracket, ']') || strchr(p_bracket, ']') - p_bracket != 6) return 0;
    strncpy(checksum, p_bracket + 1, 5);
    checksum[5] = '\0';
    
    /* Calculate Frequencies */
    for (i = 0; i < last_dash_idx; i++) {
        if (room[i] >= 'a' && room[i] <= 'z') {
            freqs[room[i] - 'a'].count++;
        }
    }
    
    /* Sort frequencies to find top 5 */
    qsort(freqs, 26, sizeof(Freq), compare_freq);
    
    /* Compare top 5 letters with checksum */
    for (i = 0; i < 5; i++) {
        if (freqs[i].letter != checksum[i]) return 0;
    }
    
    return 1;
}

/* Decrypt room name using Caesar cipher */
void decrypt_name(const char* room, int sector_id, char* output) {
    int i;
    const char* p_dash;
    int last_dash_idx;
    int shift = sector_id % 26;
    
    /* Find the last dash before the sector ID */
    p_dash = strrchr(room, '-');
    if (p_dash == NULL) {
        output[0] = '\0';
        return;
    }
    
    last_dash_idx = (int)(p_dash - room);
    
    for (i = 0; i < last_dash_idx; i++) {
        if (room[i] == '-') {
            output[i] = ' ';
        } else {
            output[i] = (room[i] - 'a' + shift) % 26 + 'a';
        }
    }
    output[last_dash_idx] = '\0';
}

/* Unit tests derived from the problem description and original Python code */
void run_tests() {
    int sector_id;
    char decrypted[MAX_ROOM_LEN];
    int pass = 1;

    cprintf("RUNNING UNIT TESTS...\r\n");

    /* Test 1: Standard valid room */
    if (validate_room("aaaaa-bbb-z-y-x-123[abxyz]", &sector_id) && sector_id == 123) {
        cprintf("TEST 1 OK (VALID ROOM)\r\n");
    } else {
        cprintf("TEST 1 FAIL\r\n");
        pass = 0;
    }

    /* Test 2: Alphabetical tie-breaker */
    if (validate_room("a-b-c-d-e-f-g-h-987[abcde]", &sector_id) && sector_id == 987) {
        cprintf("TEST 2 OK (ALPHABETICAL TIE)\r\n");
    } else {
        cprintf("TEST 2 FAIL\r\n");
        pass = 0;
    }

    /* Test 3: Complex valid room */
    if (validate_room("not-a-real-room-404[oarel]", &sector_id) && sector_id == 404) {
        cprintf("TEST 3 OK (COMPLEX FREQ)\r\n");
    } else {
        cprintf("TEST 3 FAIL\r\n");
        pass = 0;
    }

    /* Test 4: Decoy room */
    if (!validate_room("totally-real-room-200[decoy]", &sector_id)) {
        cprintf("TEST 4 OK (DECOY DETECTED)\r\n");
    } else {
        cprintf("TEST 4 FAIL\r\n");
        pass = 0;
    }

    /* Test 5: Decryption */
    decrypt_name("qzmt-zixmtkozy-ivhz-343[zimth]", 343, decrypted);
    if (strcmp(decrypted, "very encrypted name") == 0) {
        cprintf("TEST 5 OK (DECRYPTION)\r\n");
    } else {
        cprintf("TEST 5 FAIL: %s\r\n", decrypted);
        pass = 0;
    }

    if (pass) {
        cprintf("ALL TESTS PASSED!\r\n\r\n");
    } else {
        cprintf("SOME TESTS FAILED!\r\n\r\n");
    }
}

/* Sample input for algorithm verification */
const char* sample_input[] = {
    "aaaaa-bbb-z-y-x-123[abxyz]",
    "a-b-c-d-e-f-g-h-987[abcde]",
    "not-a-real-room-404[oarel]",
    "totally-real-room-200[decoy]",
    "qzmt-zixmtkozy-ivhz-343[zimth]",                  /* very encrypted name */
    "northpole-object-storage-500[oobge]",            /* northpole object storage */
    "bunny-storage-management-101[abdeg]",            /* decoy */
    "advertising-bunny-workshop-222[abcde]",          /* invalid decoy */
    NULL
};

int main() {
    int i = 0;
    long total_sector_id = 0;
    int sector_id;
    char decrypted[MAX_ROOM_LEN];
    int northpole_sector = 0;

    clrscr();
    textcolor(COLOR_WHITE);
    
    cprintf("DAY 4: SECURITY THROUGH OBSCURITY\r\n");
    cprintf("================================\r\n");

    run_tests();

    cprintf("PROCESSING SAMPLE INPUT:\r\n");
    while (sample_input[i] != NULL) {
        if (validate_room(sample_input[i], &sector_id)) {
            total_sector_id += sector_id;
            decrypt_name(sample_input[i], sector_id, decrypted);
            
            /* Print valid rooms and their decrypted names */
            cprintf("%3d: %-25s\r\n", sector_id, decrypted);
            
            /* Check for northpole */
            if (strstr(decrypted, "northpole") != NULL) {
                northpole_sector = sector_id;
            }
        }
        i++;
    }

    cprintf("--------------------------------\r\n");
    cprintf("PART 1 SUM: %ld\r\n", total_sector_id);
    cprintf("PART 2 SECTOR: %d\r\n", northpole_sector);
    
    cprintf("\r\nPRESS ANY KEY TO EXIT...");
    cgetc();

    return 0;
}
