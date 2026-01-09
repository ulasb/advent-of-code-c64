#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REPLACEMENTS 10
#define MAX_MOLECULE_LEN 64
#define MAX_GENERATED_MOLECULES 10

typedef struct {
  char from[16];
  char to[32];
} Replacement;

/* Global buffers */
char generated_molecules[MAX_GENERATED_MOLECULES][MAX_MOLECULE_LEN];
char part1_new_mol[MAX_MOLECULE_LEN];

/* Function Prototypes */
void run_tests(void);
int solve_part1(const char *molecule, Replacement replacements[], int count);
void str_replace_at(char *dest, const char *src, int pos, const char *old_str,
                    const char *new_str);

int main(void) {
  bgcolor(COLOR_BLUE);
  bordercolor(COLOR_LIGHTBLUE);
  textcolor(COLOR_WHITE);
  clrscr();
  gotoxy(0, 0);

  cprintf("ADVENT OF CODE 2015 - DAY 19\r\n");
  cprintf("MEDICINE FOR RUDOLPH\r\n");
  cprintf("============================\r\n");

  run_tests();

  cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
  cgetc();

  return 0;
}

void run_tests(void) {
  Replacement part1_replacements[3];
  int p1_res;

  cprintf("\r\nRUNNING TESTS...\r\n");

  /* Part 1 Test */
  strcpy(part1_replacements[0].from, "H");
  strcpy(part1_replacements[0].to, "HO");
  strcpy(part1_replacements[1].from, "H");
  strcpy(part1_replacements[1].to, "OH");
  strcpy(part1_replacements[2].from, "O");
  strcpy(part1_replacements[2].to, "HH");

  cprintf("TEST PART 1 (HOH): ");
  p1_res = solve_part1("HOH", part1_replacements, 3);
  if (p1_res == 4)
    cprintf("PASS\r\n");
  else
    cprintf("FAIL (got %d)\r\n", p1_res);

  cprintf("TEST PART 1 (HOHOHO): ");
  p1_res = solve_part1("HOHOHO", part1_replacements, 3);
  if (p1_res == 7)
    cprintf("PASS\r\n");
  else
    cprintf("FAIL (got %d)\r\n", p1_res);

  cprintf("\r\nNOTE: Part 2 not implemented\r\n");
  cprintf("(unstable on C64 platform)\r\n");
}

int solve_part1(const char *molecule, Replacement replacements[], int count) {
  int unique_count = 0;
  int i, j, pos;
  const char *p;
  const char *rep_from;
  const char *rep_to;
  int from_len;
  int mol_len;
  int exists;

  mol_len = strlen(molecule);

  for (i = 0; i < count; i++) {
    rep_from = replacements[i].from;
    rep_to = replacements[i].to;
    from_len = strlen(rep_from);

    p = molecule;
    while ((p = strstr(p, rep_from)) != NULL) {
      pos = p - molecule;

      /* Safety check */
      if (pos + strlen(rep_to) + (mol_len - pos - from_len) >=
          MAX_MOLECULE_LEN) {
        return -1;
      }

      str_replace_at(part1_new_mol, molecule, pos, rep_from, rep_to);

      /* Check uniqueness */
      exists = 0;
      for (j = 0; j < unique_count; j++) {
        if (strcmp(generated_molecules[j], part1_new_mol) == 0) {
          exists = 1;
          break;
        }
      }

      if (!exists && unique_count < MAX_GENERATED_MOLECULES) {
        strcpy(generated_molecules[unique_count], part1_new_mol);
        unique_count++;
      }

      p++;
    }
  }
  return unique_count;
}

void str_replace_at(char *dest, const char *src, int pos, const char *old_str,
                    const char *new_str) {
  int i;

  /* Copy prefix */
  for (i = 0; i < pos; i++) {
    dest[i] = src[i];
  }

  /* Copy new string */
  strcpy(dest + pos, new_str);

  /* Copy suffix */
  strcat(dest, src + pos + strlen(old_str));
}
