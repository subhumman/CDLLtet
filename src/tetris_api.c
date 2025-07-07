#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tetris.h"
#include <stdio.h>

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define FIGURE_SIZE 4
#define FIGURE_COUNT 7

// Classic 4x4 figures (I, O, T, S, Z, J, L)
static const int figures[FIGURE_COUNT][FIGURE_SIZE * FIGURE_SIZE] = {
    {0,0,0,0, 1,1,1,1, 0,0,0,0, 0,0,0,0}, // I
    {1,1,0,0, 1,1,0,0, 0,0,0,0, 0,0,0,0}, // O
    {0,1,0,0, 1,1,1,0, 0,0,0,0, 0,0,0,0}, // T
    {0,1,1,0, 1,1,0,0, 0,0,0,0, 0,0,0,0}, // S
    {1,1,0,0, 0,1,1,0, 0,0,0,0, 0,0,0,0}, // Z
    {1,0,0,0, 1,1,1,0, 0,0,0,0, 0,0,0,0}, // J
    {0,0,1,0, 1,1,1,0, 0,0,0,0, 0,0,0,0}  // L
};

// Game field and figure
static int field[FIELD_HEIGHT][FIELD_WIDTH];
static int cur_figure[FIGURE_SIZE * FIGURE_SIZE];
static int cur_x, cur_y, cur_type, cur_rot;
static int score = 0;
static int gameover = 0;
static int tick = 0;

// Helper functions
static void clear_field() {
    memset(field, 0, sizeof(field));
}

static void copy_figure(int type, int rot, int* out) {
    for (int i = 0; i < FIGURE_SIZE; ++i)
        for (int j = 0; j < FIGURE_SIZE; ++j) {
            int idx = i * FIGURE_SIZE + j;
            int src = 0;
            switch (rot % 4) {
                case 0: src = figures[type][idx]; break;
                case 1: src = figures[type][(FIGURE_SIZE-1-j)*FIGURE_SIZE + i]; break;
                case 2: src = figures[type][(FIGURE_SIZE*FIGURE_SIZE-1)-idx]; break;
                case 3: src = figures[type][j*FIGURE_SIZE + (FIGURE_SIZE-1-i)]; break;
            }
            out[idx] = src;
        }
}

static int check_collision(int nx, int ny, int* fig) {
    for (int i = 0; i < FIGURE_SIZE; ++i)
        for (int j = 0; j < FIGURE_SIZE; ++j) {
            if (!fig[i*FIGURE_SIZE+j]) continue;
            int x = nx + j, y = ny + i;
            if (x < 0 || x >= FIELD_WIDTH || y >= FIELD_HEIGHT) return 1;
            if (y >= 0 && field[y][x]) return 1;
        }
    return 0;
}

static void place_figure() {
    for (int i = 0; i < FIGURE_SIZE; ++i)
        for (int j = 0; j < FIGURE_SIZE; ++j)
            if (cur_figure[i*FIGURE_SIZE+j]) {
                int x = cur_x + j, y = cur_y + i;
                if (y >= 0 && y < FIELD_HEIGHT && x >= 0 && x < FIELD_WIDTH)
                    field[y][x] = 1;
            }
}

static void clear_lines() {
    for (int i = FIELD_HEIGHT-1; i >= 0; --i) {
        int full = 1;
        for (int j = 0; j < FIELD_WIDTH; ++j)
            if (!field[i][j]) { full = 0; break; }
        if (full) {
            ++score;
            for (int k = i; k > 0; --k)
                memcpy(field[k], field[k-1], sizeof(field[0]));
            memset(field[0], 0, sizeof(field[0]));
            ++i; // recheck this line
        }
    }
}

static void new_figure() {
    cur_type = rand() % FIGURE_COUNT;
    cur_rot = 0;
    copy_figure(cur_type, cur_rot, cur_figure);
    cur_x = FIELD_WIDTH/2 - 2;
    cur_y = 0;
    if (check_collision(cur_x, cur_y, cur_figure))
        gameover = 1;
}

// Exported functions
EXPORT void tetris_init() {
    srand((unsigned)time(NULL));
    clear_field();
    score = 0;
    gameover = 0;
    tick = 0;
    new_figure();
}

EXPORT void tetris_step(int action) {
    if (gameover) return;
    int temp[FIGURE_SIZE*FIGURE_SIZE];
    switch (action) {
        case 1: // rotate
            copy_figure(cur_type, cur_rot+1, temp);
            if (!check_collision(cur_x, cur_y, temp)) {
                ++cur_rot;
                copy_figure(cur_type, cur_rot, cur_figure);
            }
            break;
        case 2: // down
            if (!check_collision(cur_x, cur_y+1, cur_figure)) {
                ++cur_y;
            }
            break;
        case 3: // left
            if (!check_collision(cur_x-1, cur_y, cur_figure)) {
                --cur_x;
            }
            break;
        case 4: // right
            if (!check_collision(cur_x+1, cur_y, cur_figure)) {
                ++cur_x;
            }
            break;
    }
}

EXPORT void tetris_tick() {
    if (gameover) return;
    if (!check_collision(cur_x, cur_y+1, cur_figure)) {
        ++cur_y;
    } else {
        place_figure();
        clear_lines();
        new_figure();
    }
}

EXPORT void tetris_get_field(int* out_array) {
    for (int i = 0; i < FIELD_HEIGHT; ++i)
        for (int j = 0; j < FIELD_WIDTH; ++j)
            out_array[i*FIELD_WIDTH + j] = field[i][j];
}

EXPORT void tetris_get_figure(int* out_array) {
    int idx = 0;
    for (int i = 0; i < FIGURE_SIZE; ++i)
        for (int j = 0; j < FIGURE_SIZE; ++j)
            if (cur_figure[i*FIGURE_SIZE+j]) {
                int x = cur_x + j, y = cur_y + i;
                out_array[idx*2+0] = x;
                out_array[idx*2+1] = y;
                ++idx;
            }
    for (; idx < FIGURE_SIZE*FIGURE_SIZE; ++idx) {
        out_array[idx*2+0] = -1;
        out_array[idx*2+1] = -1;
    }
}

EXPORT int tetris_get_gameover() {
    return gameover;
}

EXPORT int tetris_get_score() {
    return score;
} 