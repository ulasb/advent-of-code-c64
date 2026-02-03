#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA_LEN 300
#define PART1_DISK_SIZE 272

/* Global buffers to save stack space */
char data_buf[MAX_DATA_LEN];
char checksum_buf[MAX_DATA_LEN];
char temp_checksum_buf[MAX_DATA_LEN];
char test_data[100];
char test_checksum[100];
char test_next[100];

/* Function Prototypes */
void run_tests(void);
void generate_dragon(char *data, int target_len);
void calculate_checksum(char *data, char *checksum, int len);
void get_final_checksum(char *buf);
void solve_part1(const char *initial_state, int disk_size);
void assert_string(const char *label, const char *actual, const char *expected);

int main(void) {
  bgcolor(COLOR_BLUE);
  bordercolor(COLOR_LIGHTBLUE);
  textcolor(COLOR_WHITE);
  clrscr();
  gotoxy(0, 0);

  cprintf("ADVENT OF CODE 2016 - DAY 16\r\n");
  cprintf("DRAGON CHECKSUM\r\n");
  cprintf("============================\r\n");

  run_tests();

  cprintf("\r\nSOLVING PART 1...\r\n");
  /* Using the input from the user's Python code */
  solve_part1("01111010110010011", PART1_DISK_SIZE);

  cprintf("\r\nNOTE: Part 2 not implemented\r\n");
  cprintf("(disk size 35M exceeds C64 memory)\r\n");

  cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
  cgetc();

  return 0;
}

void generate_dragon(char *data, int target_len) {
  int current_len = strlen(data);
  int i;
  
  while (current_len < target_len) {
    /* Step 1: Call data 'a'. Done, it's in data. */
    /* Step 2: Create 'b' as copy of 'a'. Step 3: Reverse 'b'. Step 4: Flip 'b'. */
    /* Step 5: Result is a + '0' + b. */
    
    data[current_len] = '0';
    for (i = 0; i < current_len; ++i) {
      /* b[i] is a[current_len - 1 - i] flipped */
      char val = data[current_len - 1 - i];
      data[current_len + 1 + i] = (val == '0') ? '1' : '0';
    }
    current_len = current_len * 2 + 1;
    data[current_len] = '\0';
  }
}

void calculate_checksum(char *data, char *result, int len) {
  int i;
  int res_len = 0;
  
  for (i = 0; i + 1 < len; i += 2) {
    if (data[i] == data[i+1]) {
      result[res_len++] = '1';
    } else {
      result[res_len++] = '0';
    }
  }
  result[res_len] = '\0';
}

void get_final_checksum(char *buf) {
  int cur_len = strlen(buf);
  while (cur_len % 2 == 0) {
    calculate_checksum(buf, temp_checksum_buf, cur_len);
    strcpy(buf, temp_checksum_buf);
    cur_len = strlen(buf);
  }
}

void solve_part1(const char *initial_state, int disk_size) {
  strcpy(data_buf, initial_state);
  generate_dragon(data_buf, disk_size);
  
  /* Truncate to exactly disk_size */
  data_buf[disk_size] = '\0';
  
  calculate_checksum(data_buf, checksum_buf, disk_size);
  get_final_checksum(checksum_buf);
  
  cprintf("PART 1 RESULT: %s\r\n", checksum_buf);
}

void run_tests(void) {
  cprintf("\r\nRUNNING TESTS...\r\n");

  /* Test generate */
  strcpy(test_data, "1");
  generate_dragon(test_data, 3);
  assert_string("TEST GEN (1->3)", test_data, "100");

  strcpy(test_data, "0");
  generate_dragon(test_data, 3);
  assert_string("TEST GEN (0->3)", test_data, "001");

  strcpy(test_data, "11111");
  generate_dragon(test_data, 11);
  assert_string("TEST GEN (11111->11)", test_data, "11111000000");

  strcpy(test_data, "111100001010");
  generate_dragon(test_data, 25);
  assert_string("TEST GEN (111100001010->25)", test_data, "1111000010100101011110000");

  /* Test checksum */
  calculate_checksum("110010110100", test_checksum, 12);
  assert_string("TEST CSUM (110010110100)", test_checksum, "110101");

  /* Test full process from example */
  /* initial 10000, disk 20 */
  strcpy(test_data, "10000");
  generate_dragon(test_data, 20);
  test_data[20] = '\0';
  calculate_checksum(test_data, test_checksum, 20);
  get_final_checksum(test_checksum);
  assert_string("TEST FULL (10000, 20)", test_checksum, "01100");
}

void assert_string(const char *label, const char *actual, const char *expected) {
  cprintf("%s: %s ", label, actual);
  if (strcmp(actual, expected) == 0) {
    cprintf("PASS\r\n");
  } else {
    cprintf("FAIL (expected %s)\r\n", expected);
  }
}
