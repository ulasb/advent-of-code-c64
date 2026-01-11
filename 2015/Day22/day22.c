#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Boss stats - using representative values for testing */
#define BOSS_HP 51
#define BOSS_DMG 9
#define PLAYER_START_HP 50
#define PLAYER_START_MANA 500

/* Spell costs */
#define COST_MISSILE 53
#define COST_DRAIN 73
#define COST_SHIELD 113
#define COST_POISON 173
#define COST_RECHARGE 229

typedef struct {
  signed char player_hp;
  int player_mana;
  signed char boss_hp;
  unsigned char shield_timer;
  unsigned char poison_timer;
  unsigned char recharge_timer;
  int mana_spent;
} GameState;

unsigned long states_count = 0;
unsigned int min_mana = 0xFFFF;
unsigned char hard_mode = 0;

/* Simple memoization to avoid redundant work */
#define CACHE_SIZE 128
typedef struct {
  signed char hp, boss_hp;
  unsigned char s, p, r;
} StateKey;
unsigned int cache_mana[CACHE_SIZE];
StateKey cache_key[CACHE_SIZE];

void update_progress() {
  unsigned char x = wherex();
  unsigned char y = wherey();
  gotoxy(0, 12);
  textcolor(COLOR_LIGHTBLUE);
  cprintf("states: %lu    ", states_count);
  textcolor(COLOR_WHITE);
  gotoxy(x, y);
}

void solve(GameState *s) {
  unsigned char i;
  signed char player_armor;
  GameState next;
  unsigned int cost;
  unsigned char spell_order[] = {
      3, 4, 2, 0, 1}; /* Poison, Recharge, Shield, Missile, Drain */
  unsigned char h;

  states_count++;
  if ((states_count & 0x01FF) == 0) {
    update_progress();
  }

  /* Pruning: Current mana spent + minimum possible mana to finish boss */
  /* Poison is ~10 mana/dmg, Missile is ~13. 9 is a very safe lower bound. */
  if (s->mana_spent + (s->boss_hp * 9) >= min_mana)
    return;

  /* Memoization check */
  h = (s->player_hp ^ s->boss_hp ^ s->shield_timer ^ (s->poison_timer << 2) ^
       (s->recharge_timer << 4)) &
      (CACHE_SIZE - 1);
  if (cache_key[h].hp == s->player_hp && cache_key[h].boss_hp == s->boss_hp &&
      cache_key[h].s == s->shield_timer && cache_key[h].p == s->poison_timer &&
      cache_key[h].r == s->recharge_timer) {
    if (s->mana_spent >= cache_mana[h])
      return;
  }
  cache_key[h].hp = s->player_hp;
  cache_key[h].boss_hp = s->boss_hp;
  cache_key[h].s = s->shield_timer;
  cache_key[h].p = s->poison_timer;
  cache_key[h].r = s->recharge_timer;
  cache_mana[h] = s->mana_spent;

  /* Player turn start */
  if (hard_mode) {
    s->player_hp--;
    if (s->player_hp <= 0)
      return;
  }

  /* Apply effects at start of player turn */
  player_armor = 0;
  if (s->shield_timer > 0) {
    player_armor = 7;
    s->shield_timer--;
  }
  if (s->poison_timer > 0) {
    s->boss_hp -= 3;
    s->poison_timer--;
  }
  if (s->recharge_timer > 0) {
    s->player_mana += 101;
    s->recharge_timer--;
  }

  if (s->boss_hp <= 0) {
    if ((unsigned int)s->mana_spent < min_mana)
      min_mana = (unsigned int)s->mana_spent;
    return;
  }

  /* Try casting each spell */
  for (i = 0; i < 5; ++i) {
    unsigned char spell_idx = spell_order[i];
    next = *s;

    if (spell_idx == 0) { /* Magic Missile */
      cost = COST_MISSILE;
      if (next.player_mana < cost)
        continue;
      next.player_mana -= cost;
      next.mana_spent += cost;
      next.boss_hp -= 4;
    } else if (spell_idx == 1) { /* Drain */
      cost = COST_DRAIN;
      if (next.player_mana < cost)
        continue;
      next.player_mana -= cost;
      next.mana_spent += cost;
      next.boss_hp -= 2;
      next.player_hp += 2;
    } else if (spell_idx == 2) { /* Shield */
      cost = COST_SHIELD;
      if (next.player_mana < cost || next.shield_timer > 0)
        continue;
      next.player_mana -= cost;
      next.mana_spent += cost;
      next.shield_timer = 6;
    } else if (spell_idx == 3) { /* Poison */
      cost = COST_POISON;
      if (next.player_mana < cost || next.poison_timer > 0)
        continue;
      next.player_mana -= cost;
      next.mana_spent += cost;
      next.poison_timer = 6;
    } else if (spell_idx == 4) { /* Recharge */
      cost = COST_RECHARGE;
      if (next.player_mana < cost || next.recharge_timer > 0)
        continue;
      next.player_mana -= cost;
      next.mana_spent += cost;
      next.recharge_timer = 5;
    }

    if (next.mana_spent + (next.boss_hp * 9) >= min_mana)
      continue;

    if (next.boss_hp <= 0) {
      min_mana = (unsigned int)next.mana_spent;
      continue;
    }

    /* Boss Turn */
    player_armor = 0;
    if (next.shield_timer > 0) {
      player_armor = 7;
      next.shield_timer--;
    }
    if (next.poison_timer > 0) {
      next.boss_hp -= 3;
      next.poison_timer--;
    }
    if (next.recharge_timer > 0) {
      next.player_mana += 101;
      next.recharge_timer--;
    }

    if (next.boss_hp <= 0) {
      if ((unsigned int)next.mana_spent < min_mana)
        min_mana = (unsigned int)next.mana_spent;
      continue;
    }

    /* Boss attack */
    {
      signed char dmg = BOSS_DMG - player_armor;
      if (dmg < 1)
        dmg = 1;
      next.player_hp -= dmg;
    }

    if (next.player_hp > 0) {
      solve(&next);
    }
  }
}

void main(void) {
  GameState start;

  bgcolor(COLOR_BLACK);
  bordercolor(COLOR_BLACK);
  textcolor(COLOR_WHITE);
  clrscr();

  gotoxy(0, 0);
  textcolor(COLOR_YELLOW);
  cprintf("aoc 2015 - day 22: wizard simulator\r\n");
  textcolor(COLOR_WHITE);
  cprintf("------------------------------------\r\n");
  cprintf("boss hp: %d, damage: %d\r\n", BOSS_HP, BOSS_DMG);
  cprintf("player hp: %d, mana: %d\r\n", PLAYER_START_HP, PLAYER_START_MANA);
  cprintf("------------------------------------\r\n\r\n");

  /* Part 1 */
  gotoxy(0, 6);
  cprintf("part 1: computing...");
  start.player_hp = PLAYER_START_HP;
  start.player_mana = PLAYER_START_MANA;
  start.boss_hp = BOSS_HP;
  start.shield_timer = 0;
  start.poison_timer = 0;
  start.recharge_timer = 0;
  start.mana_spent = 0;

  min_mana = 0xFFFF;
  hard_mode = 0;
  states_count = 0;
  memset(cache_key, 0xFF, sizeof(cache_key));
  solve(&start);

  gotoxy(0, 6);
  cclear(40);
  gotoxy(0, 6);
  textcolor(COLOR_WHITE);
  cprintf("part 1: ");
  textcolor(COLOR_GREEN);
  cprintf("%u", min_mana);
  textcolor(COLOR_WHITE);

  /* Part 2 */
  gotoxy(0, 8);
  cprintf("part 2: computing...");
  min_mana = 0xFFFF;
  hard_mode = 1;
  states_count = 0;
  memset(cache_key, 0xFF, sizeof(cache_key));
  solve(&start);

  gotoxy(0, 8);
  cclear(40);
  gotoxy(0, 8);
  textcolor(COLOR_WHITE);
  cprintf("part 2: ");
  textcolor(COLOR_GREEN);
  cprintf("%u", min_mana);
  textcolor(COLOR_WHITE);

  gotoxy(0, 12);
  cclear(40);
  gotoxy(0, 14);
  cprintf("press any key to exit...");
  cgetc();
}

