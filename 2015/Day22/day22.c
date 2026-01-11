#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Boss stats */
#define BOSS_HP 51
#define BOSS_DMG 9
#define PLAYER_START_HP 50
#define PLAYER_START_MANA 500

/* Spell constants */
#define COST_MISSILE 53
#define COST_DRAIN 73
#define COST_SHIELD 113
#define COST_POISON 173
#define COST_RECHARGE 229

#define MISSILE_DAMAGE 4
#define DRAIN_DAMAGE 2
#define DRAIN_HEAL 2
#define SHIELD_ARMOR 7
#define SHIELD_DURATION 6
#define POISON_DAMAGE 3
#define POISON_DURATION 6
#define RECHARGE_MANA 101
#define RECHARGE_DURATION 5

typedef struct {
  signed char player_hp;
  int player_mana;
  signed char boss_hp;
  unsigned char shield_timer;
  unsigned char poison_timer;
  unsigned char recharge_timer;
  int mana_spent;
} GameState;

typedef struct {
  unsigned int cost;
  signed char damage;
  signed char heal;
  unsigned char shield_t;
  unsigned char poison_t;
  unsigned char recharge_t;
} Spell;

const Spell spell_data[] = {
    {COST_MISSILE, MISSILE_DAMAGE, 0, 0, 0, 0},
    {COST_DRAIN, DRAIN_DAMAGE, DRAIN_HEAL, 0, 0, 0},
    {COST_SHIELD, 0, 0, SHIELD_DURATION, 0, 0},
    {COST_POISON, 0, 0, 0, POISON_DURATION, 0},
    {COST_RECHARGE, 0, 0, 0, 0, RECHARGE_DURATION}};

unsigned long states_count = 0;
unsigned int min_mana = 0xFFFF;
unsigned char hard_mode = 0;

/* Simple memoization to avoid redundant work */
#define CACHE_SIZE 128
typedef struct {
  signed char hp, boss_hp;
  unsigned char s, p, r;
  unsigned char valid;
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

void apply_effects(GameState *s, signed char *armor) {
  *armor = 0;
  if (s->shield_timer > 0) {
    *armor = SHIELD_ARMOR;
    s->shield_timer--;
  }
  if (s->poison_timer > 0) {
    s->boss_hp -= POISON_DAMAGE;
    s->poison_timer--;
  }
  if (s->recharge_timer > 0) {
    s->player_mana += RECHARGE_MANA;
    s->recharge_timer--;
  }
}

void solve(GameState *s_ptr) {
  unsigned char i;
  signed char player_armor;
  GameState next;
  GameState s;
  unsigned char h;
  const Spell *sd;
  unsigned char spell_idx;
  unsigned char spell_order[] = {
      3, 4, 2, 0, 1}; /* Poison, Recharge, Shield, Missile, Drain */

  s = *s_ptr;

  states_count++;
  if ((states_count & 0x01FF) == 0) {
    update_progress();
  }

  /* Pruning: Current mana spent + minimum possible mana to finish boss */
  /* 9 is a very safe lower bound for mana-per-damage. */
  if (s.mana_spent + (s.boss_hp * 9) >= min_mana)
    return;

  /* Memoization check */
  h = (s.player_hp ^ s.boss_hp ^ s.shield_timer ^ (s.poison_timer << 2) ^
       (s.recharge_timer << 4)) &
      (CACHE_SIZE - 1);
  if (cache_key[h].valid && cache_key[h].hp == s.player_hp &&
      cache_key[h].boss_hp == s.boss_hp && cache_key[h].s == s.shield_timer &&
      cache_key[h].p == s.poison_timer && cache_key[h].r == s.recharge_timer) {
    if (s.mana_spent >= cache_mana[h])
      return;
  }
  cache_key[h].hp = s.player_hp;
  cache_key[h].boss_hp = s.boss_hp;
  cache_key[h].s = s.shield_timer;
  cache_key[h].p = s.poison_timer;
  cache_key[h].r = s.recharge_timer;
  cache_key[h].valid = 1;
  cache_mana[h] = s.mana_spent;

  /* Player turn start */
  if (hard_mode) {
    s.player_hp--;
    if (s.player_hp <= 0)
      return;
  }

  /* Apply effects at start of player turn */
  apply_effects(&s, &player_armor);

  if (s.boss_hp <= 0) {
    if ((unsigned int)s.mana_spent < min_mana)
      min_mana = (unsigned int)s.mana_spent;
    return;
  }

  /* Try casting each spell */
  for (i = 0; i < 5; ++i) {
    spell_idx = spell_order[i];
    sd = &spell_data[spell_idx];

    /* Spell casting checks */
    if (s.player_mana < sd->cost)
      continue;
    if (sd->shield_t && s.shield_timer > 0)
      continue;
    if (sd->poison_t && s.poison_timer > 0)
      continue;
    if (sd->recharge_t && s.recharge_timer > 0)
      continue;

    next = s;
    next.player_mana -= sd->cost;
    next.mana_spent += sd->cost;

    /* Immediate effects */
    next.boss_hp -= sd->damage;
    next.player_hp += sd->heal;

    /* Over-time effects */
    if (sd->shield_t)
      next.shield_timer = sd->shield_t;
    if (sd->poison_t)
      next.poison_timer = sd->poison_t;
    if (sd->recharge_t)
      next.recharge_timer = sd->recharge_t;

    if (next.mana_spent + (next.boss_hp * 9) >= min_mana)
      continue;

    if (next.boss_hp <= 0) {
      if ((unsigned int)next.mana_spent < min_mana)
        min_mana = (unsigned int)next.mana_spent;
      continue;
    }

    /* Boss Turn: Apply effects */
    apply_effects(&next, &player_armor);

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

  /* Initialize common start state */
  start.player_hp = PLAYER_START_HP;
  start.player_mana = PLAYER_START_MANA;
  start.boss_hp = BOSS_HP;
  start.shield_timer = 0;
  start.poison_timer = 0;
  start.recharge_timer = 0;
  start.mana_spent = 0;

  /* Part 1 */
  gotoxy(0, 6);
  cprintf("part 1: computing...");
  min_mana = 0xFFFF;
  hard_mode = 0;
  states_count = 0;
  memset(cache_key, 0, sizeof(cache_key));
  solve(&start);

  gotoxy(0, 6);
  cclear(40);
  gotoxy(0, 6);
  textcolor(COLOR_WHITE);
  cprintf("part 1: ");
  textcolor(COLOR_GREEN);
  cprintf("%u", min_mana);
  textcolor(COLOR_WHITE);

  /* Part 2: Re-initialize for Hard Mode */
  gotoxy(0, 8);
  cprintf("part 2: computing...");
  start.player_hp = PLAYER_START_HP;
  start.player_mana = PLAYER_START_MANA;
  start.boss_hp = BOSS_HP;
  start.shield_timer = 0;
  start.poison_timer = 0;
  start.recharge_timer = 0;
  start.mana_spent = 0;

  min_mana = 0xFFFF;
  hard_mode = 1;
  states_count = 0;
  memset(cache_key, 0, sizeof(cache_key));
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


