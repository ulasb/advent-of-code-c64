#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHILDREN 5
#define MAX_PROGRAMS 20

typedef struct {
    char name[8];
    int weight;
    int num_children;
    char children[MAX_CHILDREN][8];
    int total_weight;
} Program;

/* Function Prototypes */
void run_tests(void);
const char* solve_part1(Program progs[], int count);
int solve_part2(Program progs[], int count);
int get_prog_idx(Program progs[], int count, const char* name);
int calculate_weights(Program progs[], int count, int root_idx);
int find_imbalance(Program progs[], int count, int node_idx, int target_weight);

int main(void) {
    bgcolor(COLOR_BLUE);
    bordercolor(COLOR_LIGHTBLUE);
    textcolor(COLOR_WHITE);
    clrscr();
    gotoxy(0, 0);

    cprintf("ADVENT OF CODE 2017 - DAY 7\r\n");
    cprintf("RECURSIVE CIRCUS\r\n");
    cprintf("============================\r\n");

    run_tests();

    cprintf("\r\nPRESS ENTER TO EXIT.\r\n");
    cgetc();

    return 0;
}

Program sample_progs[13];

void run_tests(void) {
    const char* p1_res;
    int p2_res;

    /* Initialize nodes since C89 struct array initialization can be problematic in older cc65 */
    strcpy(sample_progs[0].name, "pbga"); sample_progs[0].weight = 66; sample_progs[0].num_children = 0;
    strcpy(sample_progs[1].name, "xhth"); sample_progs[1].weight = 57; sample_progs[1].num_children = 0;
    strcpy(sample_progs[2].name, "ebii"); sample_progs[2].weight = 61; sample_progs[2].num_children = 0;
    strcpy(sample_progs[3].name, "havc"); sample_progs[3].weight = 66; sample_progs[3].num_children = 0;
    strcpy(sample_progs[4].name, "ktlj"); sample_progs[4].weight = 57; sample_progs[4].num_children = 0;

    strcpy(sample_progs[5].name, "fwft"); sample_progs[5].weight = 72; sample_progs[5].num_children = 3;
    strcpy(sample_progs[5].children[0], "ktlj"); strcpy(sample_progs[5].children[1], "cntj"); strcpy(sample_progs[5].children[2], "xhth");

    strcpy(sample_progs[6].name, "qoyq"); sample_progs[6].weight = 66; sample_progs[6].num_children = 0;

    strcpy(sample_progs[7].name, "padx"); sample_progs[7].weight = 45; sample_progs[7].num_children = 3;
    strcpy(sample_progs[7].children[0], "pbga"); strcpy(sample_progs[7].children[1], "havc"); strcpy(sample_progs[7].children[2], "qoyq");

    strcpy(sample_progs[8].name, "tknk"); sample_progs[8].weight = 41; sample_progs[8].num_children = 3;
    strcpy(sample_progs[8].children[0], "ugml"); strcpy(sample_progs[8].children[1], "padx"); strcpy(sample_progs[8].children[2], "fwft");

    strcpy(sample_progs[9].name, "jptl"); sample_progs[9].weight = 61; sample_progs[9].num_children = 0;

    strcpy(sample_progs[10].name, "ugml"); sample_progs[10].weight = 68; sample_progs[10].num_children = 3;
    strcpy(sample_progs[10].children[0], "gyxo"); strcpy(sample_progs[10].children[1], "ebii"); strcpy(sample_progs[10].children[2], "jptl");

    strcpy(sample_progs[11].name, "gyxo"); sample_progs[11].weight = 61; sample_progs[11].num_children = 0;
    strcpy(sample_progs[12].name, "cntj"); sample_progs[12].weight = 57; sample_progs[12].num_children = 0;

    cprintf("\r\nRUNNING TESTS...\r\n");

    /* Part 1 Test */
    cprintf("TEST PART 1: ");
    p1_res = solve_part1(sample_progs, 13);
    if (p1_res != NULL && strcmp(p1_res, "tknk") == 0)
        cprintf("PASS (tknk)\r\n");
    else if (p1_res != NULL)
        cprintf("FAIL (got %s)\r\n", p1_res);
    else
        cprintf("FAIL (got NULL)\r\n");

    /* Part 2 Test */
    cprintf("TEST PART 2: ");
    p2_res = solve_part2(sample_progs, 13);
    if (p2_res == 60)
        cprintf("PASS (60)\r\n");
    else
        cprintf("FAIL (got %d)\r\n", p2_res);
}

const char* solve_part1(Program progs[], int count) {
    int i, j, k;
    int is_child;

    for (i = 0; i < count; i++) {
        is_child = 0;
        for (j = 0; j < count; j++) {
            if (i == j) continue;
            for (k = 0; k < progs[j].num_children; k++) {
                if (strcmp(progs[i].name, progs[j].children[k]) == 0) {
                    is_child = 1;
                    break;
                }
            }
            if (is_child) break;
        }
        if (!is_child) {
            return progs[i].name;
        }
    }
    return NULL;
}

int get_prog_idx(Program progs[], int count, const char* name) {
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(progs[i].name, name) == 0) return i;
    }
    return -1;
}

int calculate_weights(Program progs[], int count, int root_idx) {
    int i, child_idx;
    int total = progs[root_idx].weight;

    for (i = 0; i < progs[root_idx].num_children; i++) {
        child_idx = get_prog_idx(progs, count, progs[root_idx].children[i]);
        if (child_idx >= 0) {
            total += calculate_weights(progs, count, child_idx);
        }
    }
    progs[root_idx].total_weight = total;
    return total;
}

int find_imbalance(Program progs[], int count, int node_idx, int target_weight) {
    int i, j, c, w, diff;
    int child_idx;
    int normal_weight = -1;
    int abnormal_node = -1;
    int weights[MAX_CHILDREN];
    int child_indices[MAX_CHILDREN];
    int num_children = progs[node_idx].num_children;

    if (num_children == 0) {
        diff = progs[node_idx].total_weight - target_weight;
        return progs[node_idx].weight - diff;
    }

    for (i = 0; i < num_children; i++) {
        child_idx = get_prog_idx(progs, count, progs[node_idx].children[i]);
        child_indices[i] = child_idx;
        weights[i] = progs[child_idx].total_weight;
    }

    /* Find abnormal and normal weight */
    for (i = 0; i < num_children; i++) {
        w = weights[i];
        c = 0;
        for (j = 0; j < num_children; j++) {
            if (weights[j] == w) c++;
        }
        if (c == 1) {
            abnormal_node = child_indices[i];
        } else {
            normal_weight = w;
        }
    }

    if (abnormal_node == -1 || normal_weight == -1) {
        /* All children are balanced, so the imbalance is exactly at this node */
        diff = progs[node_idx].total_weight - target_weight;
        return progs[node_idx].weight - diff;
    }

    return find_imbalance(progs, count, abnormal_node, normal_weight);
}

int solve_part2(Program progs[], int count) {
    const char* root_name = solve_part1(progs, count);
    int root_idx = get_prog_idx(progs, count, root_name);
    int i, j, w, c;
    int child_idx;
    int weights[MAX_CHILDREN];
    int child_indices[MAX_CHILDREN];
    int normal_weight = -1;
    int abnormal_node = -1;
    int num_children;

    if (root_idx == -1) return -1;

    calculate_weights(progs, count, root_idx);

    num_children = progs[root_idx].num_children;
    for (i = 0; i < num_children; i++) {
        child_idx = get_prog_idx(progs, count, progs[root_idx].children[i]);
        child_indices[i] = child_idx;
        weights[i] = progs[child_idx].total_weight;
    }

    for (i = 0; i < num_children; i++) {
        w = weights[i];
        c = 0;
        for (j = 0; j < num_children; j++) {
            if (weights[j] == w) c++;
        }
        if (c == 1) {
            abnormal_node = child_indices[i];
        } else {
            normal_weight = w;
        }
    }

    if (abnormal_node != -1 && normal_weight != -1) {
        return find_imbalance(progs, count, abnormal_node, normal_weight);
    }
    return -1;
}
