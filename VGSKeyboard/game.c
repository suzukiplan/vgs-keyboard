//
//  game.c
//  VGSKeyboard
//
//  Created by Yoji Suzuki on 2017/01/29.
//  Copyright © 2017年 SUZUKI PLAN. All rights reserved.
//
#include "vgs2.h"

int KEY_FLAG[256];

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
    draw_lane(20, 180);
    draw_keyboard(20, 180);
    return 0;
}

int vgs2_pause() {
    return 0;
}

void vgs2_term() {
}
