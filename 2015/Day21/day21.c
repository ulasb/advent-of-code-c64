#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Item structure */
typedef struct {
  const char *name;
  unsigned int cost;
  unsigned char damage;
  unsigned char armor;
} Item;

/* Item Tables */
const Item weapons[] = {{"Dagger", 8, 4, 0},
                        {"Shortsword", 10, 5, 0},
                        {"Warhammer", 25, 6, 0},
                        {"Longsword", 40, 7, 0},
                        {"Greataxe", 74, 8, 0}};

const Item armor[] = {{"None", 0, 0, 0},        {"Leather", 13, 0, 1},
                      {"Chainmail", 31, 0, 2},  {"Splintmail", 53, 0, 3},
                      {"Bandedmail", 75, 0, 4}, {"Platemail", 102, 0, 5}};

const Item rings[] = {{"None 1", 0, 0, 0},      {"None 2", 0, 0, 0},
                      {"Damage +1", 25, 1, 0},  {"Damage +2", 50, 2, 0},
                      {"Damage +3", 100, 3, 0}, {"Defense +1", 20, 0, 1},
                      {"Defense +2", 40, 0, 2}, {"Defense +3", 80, 0, 3}};

#define WEAPON_COUNT 5
#define ARMOR_COUNT 6
#define RING_COUNT 8

/* Boss Stats (Configurable) */
typedef struct {
  int hp;
  int damage;
  int armor;
} Character;

/* Battle simulation: returns 1 if player wins, 0 if boss wins */
int simulate_battle(int player_hp, int player_dmg, int player_arm,
                    const Character *boss) {
  int b_hp = boss->hp;
  int p_hp = player_hp;
  int p_dmg_done = player_dmg - boss->armor;
  int b_dmg_done = boss->damage - player_arm;

  if (p_dmg_done < 1)
    p_dmg_done = 1;
  if (b_dmg_done < 1)
    b_dmg_done = 1;

  while (1) {
    /* Player attacks */
    b_hp -= p_dmg_done;
    if (b_hp <= 0)
      return 1;

    /* Boss attacks */
    p_hp -= b_dmg_done;
    if (p_hp <= 0)
      return 0;
  }
}

void main(void) {
  unsigned int min_gold = 65535U;
  unsigned int max_gold = 0;
  unsigned char w, a, r1, r2;
  Character boss = {104, 8, 1}; /* Example/Estimated stats */

  bgcolor(COLOR_BLACK);
  bordercolor(COLOR_BLACK);
  textcolor(COLOR_WHITE);
  clrscr();

  gotoxy(0, 0);
  cprintf("RPG Simulator 20XX - Day 21\r\n");
  cprintf("---------------------------\r\n");
  cprintf("Boss: HP=%d Dmg=%d Arm=%d\r\n", boss.hp, boss.damage, boss.armor);
  cprintf("Simulating combinations...\r\n");

  for (w = 0; w < WEAPON_COUNT; ++w) {
    for (a = 0; a < ARMOR_COUNT; ++a) {
      for (r1 = 0; r1 < RING_COUNT; ++r1) {
        for (r2 = r1 + 1; r2 < RING_COUNT; ++r2) {
          unsigned int cost =
              weapons[w].cost + armor[a].cost + rings[r1].cost + rings[r2].cost;
          unsigned char p_dmg = weapons[w].damage + armor[a].damage +
                                rings[r1].damage + rings[r2].damage;
          unsigned char p_arm = weapons[w].armor + armor[a].armor +
                                rings[r1].armor + rings[r2].armor;

          if (simulate_battle(100, p_dmg, p_arm, &boss)) {
            if (cost < min_gold)
              min_gold = cost;
          } else {
            if (cost > max_gold)
              max_gold = cost;
          }
        }
      }
    }
  }

  cprintf("\r\nResults:\r\n");
  cprintf("Part 1 (Min Gold to Win): %u\r\n", min_gold);
  cprintf("Part 2 (Max Gold to Lose): %u\r\n", max_gold);

  cprintf("\r\nPress any key to exit.");
  cgetc();
}
