/*
 * Advent of Code 2016 - Day 9: Explosives in Cyberspace
 * C64 version using cc65.
 *
 * Solution created and published by Ulaş Bardak.
 * Follows Mozilla Public License 2.0.
 */

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function Prototypes */
void run_tests(void);
unsigned long get_decompressed_length_v1(const char *data);
unsigned long get_decompressed_length_v2(const char *data, unsigned int start,
                                         unsigned int end);
void parse_marker(const char *s, unsigned int *l, unsigned int *r);
void test_v1(const char *input, unsigned long expected);
void test_v2(const char *input, unsigned long expected);

int main(void) {
  bgcolor(COLOR_BLUE);
  bordercolor(COLOR_LIGHTBLUE);
  textcolor(COLOR_WHITE);
  clrscr();
  gotoxy(0, 0);

  cprintf("ADVENT OF CODE 2016 - DAY 9\r\n");
  cprintf("EXPLOSIVES IN CYBERSPACE\r\n");
  cprintf("============================\r\n");

  run_tests();

  cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
  cgetc();

  return 0;
}

void parse_marker(const char *s, unsigned int *l, unsigned int *r) {
  const char *p = s;
  *l = 0;
  while (*p >= '0' && *p <= '9') {
    *l = (*l * 10) + (*p - '0');
    p++;
  }
  if (*p == 'x')
    p++;
  *r = 0;
  while (*p >= '0' && *p <= '9') {
    *r = (*r * 10) + (*p - '0');
    p++;
  }
}

unsigned long get_decompressed_length_v1(const char *data) {
  unsigned long length = 0;
  unsigned int i = 0;
  unsigned int len = (unsigned int)strlen(data);
  unsigned int l, r;
  const char *marker_end;

  while (i < len) {
    if (data[i] == '(') {
      marker_end = strchr(data + i, ')');
      if (marker_end) {
        /* Parse LxR */
        parse_marker(data + i + 1, &l, &r);

        i = (unsigned int)(marker_end - data) + 1;
        length += (unsigned long)l * r;
        i += l;
      } else {
        length++;
        i++;
      }
    } else {
      length++;
      i++;
    }
  }
  return length;
}

unsigned long get_decompressed_length_v2(const char *data, unsigned int start,
                                         unsigned int end) {
  unsigned long length = 0;
  unsigned int i = start;
  unsigned int l, r;
  const char *marker_end;

  while (i < end) {
    if (data[i] == '(') {
      marker_end = strchr(data + i, ')');
      if (marker_end && (unsigned int)(marker_end - data) < end) {
        /* Parse LxR */
        parse_marker(data + i + 1, &l, &r);

        i = (unsigned int)(marker_end - data) + 1;
        /* Recursively calculate length of the segment */
        length += get_decompressed_length_v2(data, i, i + l) * r;
        i += l;
      } else {
        length++;
        i++;
      }
    } else {
      length++;
      i++;
    }
  }
  return length;
}

void test_v1(const char *input, unsigned long expected) {
  unsigned long res = get_decompressed_length_v1(input);
  /* Truncate long strings for display */
  if (strlen(input) > 15) {
    char buf[16];
    strncpy(buf, input, 12);
    buf[12] = '.';
    buf[13] = '.';
    buf[14] = '.';
    buf[15] = '\0';
    cprintf("%s: %lu ", buf, res);
  } else {
    cprintf("%s: %lu ", input, res);
  }

  if (res == expected) {
    cprintf("(PASS)\r\n");
  } else {
    cprintf("(FAIL: exp %lu)\r\n", expected);
  }
}

void test_v2(const char *input, unsigned long expected) {
  unsigned long res =
      get_decompressed_length_v2(input, 0, (unsigned int)strlen(input));
  /* Truncate long strings for display */
  if (strlen(input) > 20) {
    char buf[21];
    strncpy(buf, input, 17);
    buf[17] = '.';
    buf[18] = '.';
    buf[19] = '.';
    buf[20] = '\0';
    cprintf("%s: %lu ", buf, res);
  } else {
    cprintf("%s: %lu ", input, res);
  }

  if (res == expected) {
    cprintf("(PASS)\r\n");
  } else {
    cprintf("(FAIL: exp %lu)\r\n", expected);
  }
}

void run_tests(void) {
  cprintf("\r\nPART 1 TESTS:\r\n");
  test_v1("ADVENT", 6L);
  test_v1("A(1x5)BC", 7L);
  test_v1("(3x3)XYZ", 9L);
  test_v1("A(2x2)BCD(2x2)EFG", 11L);
  test_v1("(6x1)(1x3)A", 6L);
  test_v1("X(8x2)(3x3)ABCY", 18L);

  cprintf("\r\nPART 2 TESTS:\r\n");
  test_v2("(3x3)XYZ", 9L);
  test_v2("X(8x2)(3x3)ABCY", 20L);
  test_v2("(27x12)(20x12)(13x14)(7x10)(1x12)A", 241920L);
  test_v2("(25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN", 445L);
}
