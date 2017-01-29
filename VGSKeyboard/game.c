//
//  game.c
//  VGSKeyboard
//
//  Created by Yoji Suzuki on 2017/01/29.
//  Copyright © 2017年 SUZUKI PLAN. All rights reserved.
//
#include "vgs2.h"

#define MAX_NOTE 256
#define NOTES_SPEED 256

int KEY_FLAG[256];

static int POS_MAP[24] = {
    0,   // C 1
    10,  // C# 1
    20,  // D 1
    30,  // D# 1
    40,  // E 1
    60,  // F 1
    70,  // F# 1
    80,  // G 1
    90,  // G# 1
    100, // A 1
    110, // A# 1
    120, // B 1
    140, // C 2
    150, // C# 2
    160, // D 2
    170, // D# 2
    180, // E 2
    200, // F 2
    210, // F# 2
    220, // G 2
    230, // G# 2
    240, // A 2
    250, // A# 2
    260  // B 2
};

static int KEY_MAP[24] = {
    48, // C 1
    50, // D 1
    52, // E 1
    53, // F 1
    55, // G 1
    57, // A 1
    59, // B 1
    60, // C 2
    62, // D 2
    64, // E 2
    65, // F 2
    67, // G 2
    69, // A 2
    71, // B 2
    49, // C# 1
    51, // D# 1
    54, // F# 1
    56, // G# 1
    58, // A# 1
    61, // C# 2
    63, // D# 2
    66, // F# 2
    68, // G# 2
    70  // A#2
};

struct Note {
    int flag;
    int pos;
    int fy;
    int y;
    int x;
};

struct GameTable {
    struct Note n[MAX_NOTE];
    int n_idx;
} TBL;

void add_note(int pos) {
    TBL.n[TBL.n_idx].flag = 1;
    TBL.n[TBL.n_idx].pos = pos;
    TBL.n[TBL.n_idx].x = POS_MAP[pos] + 20;
    TBL.n[TBL.n_idx].y = -4;
    TBL.n[TBL.n_idx].fy = TBL.n[TBL.n_idx].y * 256;
    TBL.n_idx++;
    TBL.n_idx &= MAX_NOTE - 1;
}

void draw_notes() {
    int i;
    for (i = 0; i < MAX_NOTE; i++) {
        if (TBL.n[i].flag) {
            vgs2_boxfSP(TBL.n[i].x, TBL.n[i].y, TBL.n[i].x + 16, TBL.n[i].y + 4, 111);
            TBL.n[i].fy += NOTES_SPEED;
            TBL.n[i].y = TBL.n[i].fy >> 8;
            if (180 < TBL.n[i].y) {
                TBL.n[i].flag = 0;
            }
        }
    }
}

void draw_lane(int x, int y) {
    int i;
    int n = 0;
    for (i = 0; i < 14; i++, n++) {
        vgs2_boxfSP(i * 20 + x + 8, 0, i * 20 + x + 9, y, KEY_FLAG[KEY_MAP[n]] ? 51 : 50);
    }
    x += 10;
    for (i = 0; i < 2; i++, n++) {
        vgs2_boxfSP(i * 20 + x + 8, 0, i * 20 + x + 9, y, KEY_FLAG[KEY_MAP[n]] ? 10 : 9);
    }
    x += 60;
    for (i = 0; i < 3; i++, n++) {
        vgs2_boxfSP(i * 20 + x + 8, 0, i * 20 + x + 9, y, KEY_FLAG[KEY_MAP[n]] ? 10 : 9);
    }
    x += 80;
    for (i = 0; i < 2; i++, n++) {
        vgs2_boxfSP(i * 20 + x + 8, 0, i * 20 + x + 9, y, KEY_FLAG[KEY_MAP[n]] ? 10 : 9);
    }
    x += 60;
    for (i = 0; i < 3; i++, n++) {
        vgs2_boxfSP(i * 20 + x + 8, 0, i * 20 + x + 9, y, KEY_FLAG[KEY_MAP[n]] ? 10 : 9);
    }
}

void draw_keyboard(int x, int y) {
    int i;
    int n = 0;
    for (i = 0; i < 14; i++, n++) {
        vgs2_boxfSP(i * 20 + x, y, i * 20 + x + 18, y + 56, KEY_FLAG[KEY_MAP[n]] ? 31 : 111);
    }
    x += 10;
    for (i = 0; i < 2; i++, n++) {
        vgs2_boxfSP(i * 20 + x + 1, y, i * 20 + x + 15, y + 41, 1);
        vgs2_boxfSP(i * 20 + x + 2, y, i * 20 + x + 14, y + 40, KEY_FLAG[KEY_MAP[n]] ? 31 : 96);
    }
    x += 60;
    for (i = 0; i < 3; i++, n++) {
        vgs2_boxfSP(i * 20 + x + 1, y, i * 20 + x + 15, y + 41, 1);
        vgs2_boxfSP(i * 20 + x + 2, y, i * 20 + x + 14, y + 40, KEY_FLAG[KEY_MAP[n]] ? 31 : 96);
    }
    x += 80;
    for (i = 0; i < 2; i++, n++) {
        vgs2_boxfSP(i * 20 + x + 1, y, i * 20 + x + 15, y + 41, 1);
        vgs2_boxfSP(i * 20 + x + 2, y, i * 20 + x + 14, y + 40, KEY_FLAG[KEY_MAP[n]] ? 31 : 96);
    }
    x += 60;
    for (i = 0; i < 3; i++, n++) {
        vgs2_boxfSP(i * 20 + x + 1, y, i * 20 + x + 15, y + 41, 1);
        vgs2_boxfSP(i * 20 + x + 2, y, i * 20 + x + 14, y + 40, KEY_FLAG[KEY_MAP[n]] ? 31 : 96);
    }
}

int vgs2_init() {
    vgs2_pallet256(0);
    return 0;
}

int vgs2_loop() {
    static unsigned int frame;
    frame++;
    if (frame % 48 == 0) add_note(vgs2_rand() % 24);
    draw_lane(20, 180);
    draw_notes();
    draw_keyboard(20, 180);
    return 0;
}

int vgs2_pause() {
    return 0;
}

void vgs2_term() {
}
