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

#define HITCHK(X1, Y1, XS1, YS1, X2, Y2, XS2, YS2) (X1 < X2 + XS2 && X2 < X1 + XS1 && Y1 < Y2 + YS2 && Y2 < Y1 + YS1)

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

static int KEY_MAP2[24] = {
    48, // C 1
    49, // C# 1
    50, // D 1
    51, // D# 1
    52, // E 1
    53, // F 1
    54, // F# 1
    55, // G 1
    56, // G# 1
    57, // A 1
    58, // A# 1
    59, // B 1
    60, // C 2
    61, // C# 2
    62, // D 2
    63, // D# 2
    64, // E 2
    65, // F 2
    66, // F# 2
    67, // G 2
    68, // G# 2
    69, // A 2
    70, // A#2
    71  // B 2

};

struct Note {
    int flag;
    int pos;
    int fy;
    int y;
    int x;
    int gx;
};

struct Bomb {
    int flag;
    int an;
    int x;
    int y;
};

struct ScoreElement {
    int pos;
    int interval;
};

#define L4 40

struct ScoreElement KAERU[] = {
    {12, L4}, {14, L4}, {16, L4}, {17, L4}, {16, L4}, {14, L4}, {12, L4 * 2},
    {16, L4}, {17, L4}, {19, L4}, {21, L4}, {19, L4}, {17, L4}, {16, L4 * 2},
    {12, L4 * 2}, {12, L4 * 2}, {12, L4 * 2}, {12, L4 * 2},
    {12, L4 / 2}, {12, L4 / 2}, {14, L4 / 2}, {14, L4 / 2}, {16, L4 / 2}, {16, L4 / 2},
    {17, L4 / 2}, {17, L4 / 2}, {16, L4}, {14, L4}, {12, L4 * 4},
    {0, 0}
};

struct GameTable {
    struct Note n[MAX_NOTE];
    int n_idx;
    struct Bomb b[MAX_NOTE];
    int b_idx;
    int push[24];
    int prev[24];
    int spos;
    int z;
} TBL;

void push_check() {
    int i, now;
    for (i = 0; i < 24; i++) {
        now = KEY_FLAG[KEY_MAP2[i]];
        if (now && !TBL.prev[i]) {
            TBL.push[i] = 1;
        } else {
            TBL.push[i] = 0;
        }
        TBL.prev[i] = now;
    }
}

void add_bomb(int pos, int y) {
    TBL.b[TBL.b_idx].flag = 1;
    TBL.b[TBL.b_idx].an = 0;
    TBL.b[TBL.b_idx].x = POS_MAP[pos] + 20 + 1 - 4;
    TBL.b[TBL.b_idx].y = y - 10;
    TBL.b_idx++;
    TBL.b_idx &= MAX_NOTE - 1;
}

void draw_bomb() {
    int i;
    for (i = 0; i < MAX_NOTE; i++) {
        if (TBL.b[i].flag) {
            vgs2_putSP(2, TBL.b[i].an / 2 * 24, 0, 24, 24, TBL.b[i].x, TBL.b[i].y);
            TBL.b[i].an++;
            if (20 <= TBL.b[i].an) {
                TBL.b[i].flag = 0;
            }
        }
    }
}

void add_note(int pos) {
    TBL.n[TBL.n_idx].flag = 1;
    TBL.n[TBL.n_idx].pos = pos;
    TBL.n[TBL.n_idx].x = POS_MAP[pos] + 20 + 1;
    TBL.n[TBL.n_idx].y = -4;
    TBL.n[TBL.n_idx].fy = TBL.n[TBL.n_idx].y * 256;
    switch (pos) {
        case 1:
        case 3:
        case 6:
        case 8:
        case 10:
        case 1 + 12:
        case 3 + 12:
        case 6 + 12:
        case 8 + 12:
        case 10 + 12:
            TBL.n[TBL.n_idx].gx = 16;
            break;
        default:
            TBL.n[TBL.n_idx].gx = 0;
    }
    TBL.n_idx++;
    TBL.n_idx &= MAX_NOTE - 1;
}

void draw_notes() {
    int i;
    for (i = 0; i < MAX_NOTE; i++) {
        if (TBL.n[i].flag) {
            vgs2_putSP(1, TBL.n[i].gx, 0, 16, 4, TBL.n[i].x, TBL.n[i].y);
            if (TBL.push[TBL.n[i].pos]) {
                if (HITCHK(TBL.n[i].x, TBL.n[i].y, 16, 4, 0, 176, 320, 4)) {
                    // perfect
                    puts("perfect");
                    TBL.n[i].flag = 0;
                    add_bomb(TBL.n[i].pos, TBL.n[i].y);
                } else if (HITCHK(TBL.n[i].x, TBL.n[i].y, 16, 4, 0, 174, 320, 8)) {
                    // great
                    puts("great");
                    TBL.n[i].flag = 0;
                    add_bomb(TBL.n[i].pos, TBL.n[i].y);
                } else if (HITCHK(TBL.n[i].x, TBL.n[i].y, 16, 4, 0, 172, 320, 12)) {
                    // good
                    puts("good");
                    TBL.n[i].flag = 0;
                    add_bomb(TBL.n[i].pos, TBL.n[i].y);
                } else if (HITCHK(TBL.n[i].x, TBL.n[i].y, 16, 4, 0, 170, 320, 16)) {
                    // bad
                    puts("bad");
                    TBL.n[i].flag = 0;
                    add_bomb(TBL.n[i].pos, TBL.n[i].y);
                }
            }
            if (TBL.n[i].flag) {
                TBL.n[i].fy += NOTES_SPEED;
                TBL.n[i].y = TBL.n[i].fy >> 8;
                if (200 < TBL.n[i].y) {
                    TBL.n[i].flag = 0;
                    // miss
                    puts("miss");
                }
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
    if (0 == frame) {
        add_note(KAERU[TBL.spos].pos + TBL.z);
    }
    frame++;
    if (KAERU[TBL.spos].interval <= frame) {
        frame = 0;
        TBL.spos++;
        if (!KAERU[TBL.spos].interval) {
            TBL.spos = 0;
            TBL.z--;
            if (-12 == TBL.z) {
                TBL.z = 0;
            }
        }
    }
    push_check();
    draw_lane(20, 180);
    vgs2_boxSP(0, 176, 320, 179, 105);
    draw_notes();
    draw_bomb();
    draw_keyboard(20, 180);
    return 0;
}

int vgs2_pause() {
    return 0;
}

void vgs2_term() {
}
