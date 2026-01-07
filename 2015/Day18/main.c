#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 100
// Add padding for sentinel borders (1 on each side)
#define BUFFER_SIZE (MAX_SIZE + 2)

// Globals to store grid state
// 0 = OFF, 1 = ON
static unsigned char current_grid[BUFFER_SIZE][BUFFER_SIZE];
static unsigned char next_grid[BUFFER_SIZE][BUFFER_SIZE];

static int grid_width = 0;
static int grid_height = 0;

// Sample 6x6 input for testing or fallback
const char *sample_input[] = {".#.#.#", "...##.", "#....#",
                              "..#...", "#.#..#", "####.."};

// Initialize grids to 0 (all off)
void clear_grids() {
  memset(current_grid, 0, sizeof(current_grid));
  memset(next_grid, 0, sizeof(next_grid));
}

// Load sample data
void load_sample() {
  int r, c;
  grid_width = 6;
  grid_height = 6;
  clear_grids();

  printf("Loading sample 6x6 grid...\n");
  for (r = 0; r < 6; ++r) {
    for (c = 0; c < 6; ++c) {
      if (sample_input[r][c] == '#') {
        current_grid[r + 1][c + 1] = 1;
      }
    }
  }
}

// Try to load input.txt
int load_file(const char *filename) {
  FILE *fp;
  char buffer[120]; // ample space for 100 chars + newline
  int r = 1;

  fp = fopen(filename, "r");
  if (!fp) {
    return 0;
  }

  clear_grids();

  while (fgets(buffer, sizeof(buffer), fp)) {
    int len = strlen(buffer);
    int c;
    // Trim newline
    while (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n')) {
      buffer[--len] = 0;
    }

    if (len == 0)
      continue;

    if (grid_width == 0)
      grid_width = len;

    for (c = 0; c < len; ++c) {
      if (buffer[c] == '#') {
        current_grid[r][c + 1] = 1;
      }
    }
    r++;
    if (r > MAX_SIZE)
      break;
  }
  grid_height = r - 1;

  fclose(fp);
  return 1;
}

// Count neighbors for cell at (r, c)
// Because of sentinel borders, we don't need boundary checks!
unsigned char count_neighbors(int r, int c) {
  // 8 neighbors
  // Top row
  unsigned char count = current_grid[r - 1][c - 1] + current_grid[r - 1][c] +
                        current_grid[r - 1][c + 1];
  // Middle row (skip self)
  count += current_grid[r][c - 1] + current_grid[r][c + 1];
  // Bottom row
  count += current_grid[r + 1][c - 1] + current_grid[r + 1][c] +
           current_grid[r + 1][c + 1];

  return count;
}

// Enforce corner lights ON (Part 2)
void fix_corners() {
  current_grid[1][1] = 1;
  current_grid[1][grid_width] = 1;
  current_grid[grid_height][1] = 1;
  current_grid[grid_height][grid_width] = 1;
}

// Run one simulation step
void step_simulation(int fix_corners_mode) {
  int r, c;

  if (fix_corners_mode) {
    fix_corners();
  }

  for (r = 1; r <= grid_height; ++r) {
    for (c = 1; c <= grid_width; ++c) {
      unsigned char n = count_neighbors(r, c);
      unsigned char state = current_grid[r][c];

      if (state == 1) {
        // Stay on if 2 or 3 neighbors
        if (n == 2 || n == 3) {
          next_grid[r][c] = 1;
        } else {
          next_grid[r][c] = 0;
        }
      } else {
        // Turn on if exactly 3 neighbors
        if (n == 3) {
          next_grid[r][c] = 1;
        } else {
          next_grid[r][c] = 0;
        }
      }
    }
  }

  // Copy next to current
  // We copy only the active region to save time, or just swap pointers?
  // Pointers are harder with static 2D arrays (contiguous memory block
  // requirement). Just memcpy or manual copy. Memcpy is faster often. grid size
  // is (MAX_SIZE+2)*(MAX_SIZE+2) bytes total? No, we only care about 1..H, 1..W
  // But we must ensure borders are clean if we rely on them being 0?
  // Next_grid borders are 0 initialized and never touched loop.
  // So we just copy the inner part.

  for (r = 1; r <= grid_height; ++r) {
    // Copy row
    // memcpy(&current_grid[r][1], &next_grid[r][1], grid_width);
    // Manual loop might be safer/simpler without relying on string.h overhead
    // if custom
    for (c = 1; c <= grid_width; ++c) {
      current_grid[r][c] = next_grid[r][c];
    }
  }

  if (fix_corners_mode) {
    fix_corners();
  }
}

int count_lights() {
  int count = 0;
  int r, c;
  for (r = 1; r <= grid_height; ++r) {
    for (c = 1; c <= grid_width; ++c) {
      if (current_grid[r][c])
        count++;
    }
  }
  return count;
}

void run_part1(const char *input_file) {
  int i;
  int steps;
  clock_t t0, t1;

  if (!load_file(input_file)) {
    load_sample();
    steps = 4; // Sample uses 4 steps
  } else {
    printf("Loaded %dx%d grid from %s\n", grid_width, grid_height, input_file);
    steps = 100; // Real input uses 100 steps
  }

  printf("Running Part 1 (%d steps)...\n", steps);
  t0 = clock();

  for (i = 0; i < steps; ++i) {
    step_simulation(0);
    // specific optimization: print dot every 10 steps to show progress
    if (i % 10 == 0) {
      printf(".");
    }
  }
  printf("\n");

  t1 = clock();
  printf("Part 1 Result: %d lights on\n", count_lights());
  printf("Time: %ld ticks\n", (long)(t1 - t0));
}

void run_part2(const char *input_file) {
  int i;
  int steps;

  if (!load_file(input_file)) {
    load_sample();
    steps = 5; // Sample part 2 uses 5 steps
  } else {
    printf("Loaded %dx%d grid from %s\n", grid_width, grid_height, input_file);
    steps = 100;
  }

  // Explicitly turn on corners initially for Part 2
  fix_corners();

  printf("Running Part 2 (%d steps)...\n", steps);

  for (i = 0; i < steps; ++i) {
    step_simulation(1);
    if (i % 10 == 0)
      printf(".");
  }
  printf("\n");

  printf("Part 2 Result: %d lights on\n", count_lights());
}

int main() {
  printf("AoC 2015 - Day 18\n");

  run_part1("input.txt");
  run_part2("input.txt");

  return 0;
}
