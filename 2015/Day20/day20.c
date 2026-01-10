#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Scaled down for C64 performance demonstration */
#define MIN_PRESENTS_GOAL 50000L

/* Function Prototypes */
typedef unsigned long (*calc_func)(unsigned long);
unsigned long calculate_presents_part1(unsigned long house);
unsigned long calculate_presents_part2(unsigned long house);
void solve(const char *part_name, calc_func calculate);

int main(void) {
  bgcolor(COLOR_BLUE);
  bordercolor(COLOR_LIGHTBLUE);
  textcolor(COLOR_WHITE);
  clrscr();
  gotoxy(0, 0);

  cprintf("ADVENT OF CODE 2015 - DAY 20\r\n");
  cprintf("INFINITE ELVES & HOUSES (LITE)\r\n");
  cprintf("==============================\r\n");
  cprintf("TARGET: %lu PRESENTS\r\n\r\n", MIN_PRESENTS_GOAL);

  solve("PART 1", calculate_presents_part1);
  cprintf("\r\n");
  solve("PART 2", calculate_presents_part2);

  cprintf("\r\nPRESS ANY KEY TO EXIT.\r\n");
  cgetc();

  return 0;
}

void solve(const char *part_name, calc_func calculate) {
  unsigned long house = 1;
  unsigned long presents;

  cprintf("SOLVING %s...\r\n", part_name);
  for (house = 1;; ++house) {
    presents = calculate(house);
    if (presents >= MIN_PRESENTS_GOAL)
      break;
    if (house % 100 == 0)
      cprintf(".");
  }
  cprintf("\r\n%s: HOUSE %lu (%lu PRESENTS)\r\n", part_name, house, presents);
}

/*
 * Each elf 'i' delivers 10*i presents to every multiple of 'i'.
 * For house 'H', we sum 10*i for all divisors 'i' of 'H'.
 */
unsigned long calculate_presents_part1(unsigned long house) {
  unsigned long sum = 0;
  unsigned long i;
  for (i = 1; i * i <= house; i++) {
    if (house % i == 0) {
      sum += i;
      if (i * i != house)
        sum += (house / i);
    }
  }
  return sum * 10;
}

/*
 * Each elf 'i' delivers 11*i presents to 50 houses.
 * For house 'H', we sum 11*i for divisors 'i' where H/i <= 50.
 */
unsigned long calculate_presents_part2(unsigned long house) {
  unsigned long sum = 0;
  unsigned long i;
  for (i = 1; i * i <= house; i++) {
    if (house % i == 0) {
      /* Divisor i (Elf ID) */
      if (house / i <= 50)
        sum += i;

      /* Divisor house/i (Elf ID) */
      if (i * i != house) {
        unsigned long other = house / i;
        if (i <= 50)
          sum += other;
      }
    }
  }
  return sum * 11;
}
