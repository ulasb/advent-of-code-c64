#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REGISTERS 16
#define MAX_NAME_LEN 8

typedef struct {
  char name[MAX_NAME_LEN];
  long value;
} Register;

Register registers[MAX_REGISTERS];
int reg_count = 0;
long max_ever = 0;

long get_reg_val(const char *name) {
  int i;
  for (i = 0; i < reg_count; i++) {
    if (strcmp(registers[i].name, name) == 0) {
      return registers[i].value;
    }
  }
  /* Not found, create it with value 0 */
  if (reg_count < MAX_REGISTERS) {
    strncpy(registers[reg_count].name, name, MAX_NAME_LEN - 1);
    registers[reg_count].name[MAX_NAME_LEN - 1] = '\0';
    registers[reg_count].value = 0;
    return registers[reg_count++].value;
  }
  return 0;
}

void set_reg_val(const char *name, long val) {
  int i;
  for (i = 0; i < reg_count; i++) {
    if (strcmp(registers[i].name, name) == 0) {
      registers[i].value = val;
      if (val > max_ever)
        max_ever = val;
      return;
    }
  }
  /* Not found, create it */
  if (reg_count < MAX_REGISTERS) {
    strncpy(registers[reg_count].name, name, MAX_NAME_LEN - 1);
    registers[reg_count].name[MAX_NAME_LEN - 1] = '\0';
    registers[reg_count].value = val;
    if (val > max_ever)
      max_ever = val;
    reg_count++;
  }
}

int evaluate_condition(const char *reg_name, const char *op, long val) {
  long reg_val = get_reg_val(reg_name);

  if (strcmp(op, ">") == 0) return reg_val > val;
  if (strcmp(op, "<") == 0) return reg_val < val;
  if (strcmp(op, ">=") == 0) return reg_val >= val;
  if (strcmp(op, "<=") == 0) return reg_val <= val;
  if (strcmp(op, "==") == 0) return reg_val == val;
  if (strcmp(op, "!=") == 0) return reg_val != val;

  return 0;
}

void run_instruction(const char *line) {
  char target[MAX_NAME_LEN];
  char op[4];
  long amount;
  char if_token[3];
  char cond_reg[MAX_NAME_LEN];
  char cond_op[4];
  long cond_val;

  /* Format: b inc 5 if a > 1 */
  sscanf(line, "%s %s %ld %s %s %s %ld", target, op, &amount, if_token, cond_reg,
         cond_op, &cond_val);

  if (evaluate_condition(cond_reg, cond_op, cond_val)) {
    long current_val = get_reg_val(target);
    if (strcmp(op, "inc") == 0) {
      set_reg_val(target, current_val + amount);
    } else if (strcmp(op, "dec") == 0) {
      set_reg_val(target, current_val - amount);
    }
  }
}

void print_registers(void) {
  int i;
  cprintf("\r\nREGISTERS:\r\n");
  for (i = 0; i < reg_count; i++) {
    cprintf("%s: %ld\r\n", registers[i].name, registers[i].value);
  }
}

long get_max_after(void) {
  int i;
  long max_val = 0;
  if (reg_count == 0) return 0;
  max_val = registers[0].value;
  for (i = 1; i < reg_count; i++) {
    if (registers[i].value > max_val) {
      max_val = registers[i].value;
    }
  }
  return max_val;
}

int main(void) {
  const char *sample_input[] = {
      "b inc 5 if a > 1",
      "a inc 1 if b < 5",
      "c dec -10 if a >= 1",
      "c inc -20 if c == 10"
  };
  int i;

  bgcolor(COLOR_BLACK);
  bordercolor(COLOR_GRAY3);
  textcolor(COLOR_GREEN);
  clrscr();
  gotoxy(0, 0);

  cprintf("ADVENT OF CODE 2017 - DAY 8\r\n");
  cprintf("I HEARD YOU LIKE REGISTERS\r\n");
  cprintf("============================\r\n");

  cprintf("\r\nRUNNING SAMPLE INSTRUCTIONS...\r\n");
  for (i = 0; i < 4; i++) {
    cprintf("> %s\r\n", sample_input[i]);
    run_instruction(sample_input[i]);
  }

  print_registers();

  cprintf("\r\nRESULTS:\r\n");
  cprintf("PART 1 (MAX AFTER): %ld\r\n", get_max_after());
  cprintf("PART 2 (MAX EVER):  %ld\r\n", max_ever);

  cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
  cgetc();

  return 0;
}
