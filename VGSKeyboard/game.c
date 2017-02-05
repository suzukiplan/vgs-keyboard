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
    int len;
};

struct Bomb {
    int flag;
    int an;
    int x;
    int y;
};

struct Result {
    int flag;
    int type;
    int an;
};

struct ScoreElement {
    int pos;
    int len;
    int interval;
};

#define L16 10
#define L8 20
#define L4 40
#define L2 80
#define L1 160

struct ScoreElement KAERU[] = {
    {12, L8, L4}, {14, L8, L4}, {16, L8, L4}, {17, L8, L4}, {16, L8, L4}, {14, L8, L4}, {12, L4, L2},
    {16, L8, L4}, {17, L8, L4}, {19, L8, L4}, {21, L8, L4}, {19, L8, L4}, {17, L8, L4}, {16, L4, L2},
    {12, 0, L2}, {12, 0, L2}, {12, 0, L2}, {12, 0, L2},
    {12, 0, L8}, {12, 0, L8}, {14, 0, L8}, {14, 0, L8}, {16, 0, L8}, {16, 0, L8},
    {17, 0, L8}, {17, 0, L8}, {16, L8, L4}, {14, L8, L4}, {12, L2, L1},
    {0, 0, 0}
};

struct GameTable {
    struct Note n[MAX_NOTE];
    int n_idx;
    struct Bomb b[MAX_NOTE];
    int b_idx;
    int keyDown[24];
    int prevDown[24];
    int keyUp[24];
    int prev[24];
    struct Result r;
    int spos;
    int z;
    int combo;
} TBL;

void myprint(int x, int y, const char *msg, ...)
{
    char buf[256];
    int i;
    int c;
    int d;
    va_list args;
    
    va_start(args, msg);
    vsprintf(buf, msg, args);
    va_end(args);
    
    for (i = 0; '\0' != (c = (int)buf[i]); i++, x += 8) {
        c -= 0x20;
        c &= 0x7f;
        d = c >> 4;
        vgs2_putSP(0, (c - (d << 4)) << 3, d << 3, 8, 8, x, y);
    }
}

void myprintS(int x, int y, const char *msg, ...)
{
    char buf[256];
    int i;
    int c;
    int d;
    va_list args;
    
    va_start(args, msg);
    vsprintf(buf, msg, args);
    va_end(args);
    
    for (i = 0; '\0' != (c = (int)buf[i]); i++, x += 4) {
        c -= 0x20;
        c &= 0x7f;
        d = c >> 5;
        vgs2_putSP(0, (c - (d << 5)) << 2, 48 + (d << 3), 4, 8, x, y);
    }
}

void show_result(int type) {
    TBL.r.flag = 1;
    TBL.r.type = type;
    TBL.r.an = 0;
    if (3 <= type) {
        TBL.combo++;
    } else {
        TBL.combo = 0;
    }
}

void draw_result() {
    if (TBL.r.flag) {
        int ys;
        int xs = TBL.r.an * 8;
        if (xs < 0) {
            xs = 1;
        } else if (80 < xs) {
            xs = 80;
        }
        ys = xs * 3 / 10;
        if (ys) {
            vgs2_putSPE(3, 0, TBL.r.type * 24, 80, 24, (320 - xs) / 2, (240 - ys) / 2 - 80, xs, ys);
        }
        TBL.r.an++;
        if (30 < TBL.r.an) {
            TBL.r.flag = 0;
        }
    }
}

void push_check() {
    int i, now;
    for (i = 0; i < 24; i++) {
        now = KEY_FLAG[KEY_MAP2[i]];
        if (now) {
            if (!TBL.prev[i]) {
                TBL.keyDown[i] = 1;
            } else {
                TBL.keyDown[i] = 0;
            }
        } else {
            if (TBL.prev[i]) {
                TBL.keyUp[i] = 1;
            } else {
                TBL.keyUp[i] = 0;
            }
        }
        TBL.prev[i] = now;
    }
}

void draw_combo() {
    if (2 <= TBL.combo) {
        myprint(224, 20, "%5d COMBO", TBL.combo);
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

void add_note(int pos, int len) {
    TBL.n[TBL.n_idx].flag = 1;
    TBL.n[TBL.n_idx].pos = pos;
    TBL.n[TBL.n_idx].len = len;
    TBL.n[TBL.n_idx].x = POS_MAP[pos] + 20 + 3;
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
    int i, ey, j;
    for (i = 0; i < MAX_NOTE; i++) {
        if (TBL.n[i].flag) {
            if (TBL.n[i].len) {
                ey = TBL.n[i].y - ((NOTES_SPEED * TBL.n[i].len) >> 8) - 4;
                if (TBL.n[i].gx) {
                    for (j = 0; j < 7; j++) {
                        vgs2_lineSP(TBL.n[i].x + 7 - j, ey + 4, TBL.n[i].x + 7 - j, TBL.n[i].y, 60 - j * 2);
                        vgs2_lineSP(TBL.n[i].x + 8 + j, ey + 4, TBL.n[i].x + 8 + j, TBL.n[i].y, 60 - j * 2);
                    }
                } else {
                    for (j = 0; j < 7; j++) {
                        vgs2_lineSP(TBL.n[i].x + 7 - j, ey + 4, TBL.n[i].x + 7 - j, TBL.n[i].y, 76 - j * 2);
                        vgs2_lineSP(TBL.n[i].x + 8 + j, ey + 4, TBL.n[i].x + 8 + j, TBL.n[i].y, 76 - j * 2);
                    }
                }
                vgs2_putSP(1, 32, 0, 16, 4, TBL.n[i].x, TBL.n[i].y);
                vgs2_putSP(1, 32, 0, 16, 4, TBL.n[i].x, ey);
                if (1 == TBL.n[i].flag) {
                    if (TBL.keyDown[TBL.n[i].pos]) {
                        if (HITCHK(TBL.n[i].x, TBL.n[i].y, 16, 4, 0, 176, 320, 4)) {
                            // perfect
                            show_result(4);
                            TBL.n[i].flag++;
                        } else if (HITCHK(TBL.n[i].x, TBL.n[i].y, 16, 4, 0, 174, 320, 8)) {
                            // great
                            show_result(3);
                            TBL.n[i].flag++;
                        } else if (HITCHK(TBL.n[i].x, TBL.n[i].y, 16, 4, 0, 172, 320, 12)) {
                            // good
                            show_result(2);
                            TBL.n[i].flag++;
                        } else if (HITCHK(TBL.n[i].x, TBL.n[i].y, 16, 4, 0, 170, 320, 16)) {
                            // bad
                            show_result(1);
                            TBL.n[i].flag++;
                        }
                    }
                } else if (2 == TBL.n[i].flag) {
                    if (TBL.keyUp[TBL.n[i].pos]) {
                        if (HITCHK(TBL.n[i].x, ey, 16, 4, 0, 176, 320, 4)) {
                            // perfect
                            show_result(4);
                            TBL.n[i].flag = 0;
                            add_bomb(TBL.n[i].pos, ey);
                        } else if (HITCHK(TBL.n[i].x, ey, 16, 4, 0, 174, 320, 8)) {
                            // great
                            show_result(3);
                            TBL.n[i].flag = 0;
                            add_bomb(TBL.n[i].pos, ey);
                        } else if (HITCHK(TBL.n[i].x, ey, 16, 4, 0, 172, 320, 12)) {
                            // good
                            show_result(2);
                            TBL.n[i].flag = 0;
                        } else if (HITCHK(TBL.n[i].x, ey, 16, 4, 0, 170, 320, 16)) {
                            // bad
                            show_result(1);
                            TBL.n[i].flag = 0;
                        } else {
                            // miss
                            show_result(0);
                            TBL.n[i].flag = 0;
                        }
                    }
                }
                if (TBL.n[i].flag) {
                    TBL.n[i].fy += NOTES_SPEED;
                    TBL.n[i].y = TBL.n[i].fy >> 8;
                    if (1 == TBL.n[i].flag) {
                        if (200 < TBL.n[i].y) {
                            TBL.n[i].flag = 0;
                            // miss
                            show_result(0);
                        }
                    } else if (2 == TBL.n[i].flag) {
                        ey = TBL.n[i].y - ((NOTES_SPEED * TBL.n[i].len) >> 8) - 4;
                        if (200 < ey) {
                            TBL.n[i].flag = 0;
                            // miss
                            show_result(0);
                        }
                    }
                }
            } else {
                vgs2_putSP(1, TBL.n[i].gx, 0, 16, 4, TBL.n[i].x, TBL.n[i].y);
                if (TBL.keyDown[TBL.n[i].pos]) {
                    if (HITCHK(TBL.n[i].x, TBL.n[i].y, 16, 4, 0, 176, 320, 4)) {
                        // perfect
                        show_result(4);
                        TBL.n[i].flag = 0;
                        add_bomb(TBL.n[i].pos, TBL.n[i].y);
                    } else if (HITCHK(TBL.n[i].x, TBL.n[i].y, 16, 4, 0, 174, 320, 8)) {
                        // great
                        show_result(3);
                        TBL.n[i].flag = 0;
                        add_bomb(TBL.n[i].pos, TBL.n[i].y);
                    } else if (HITCHK(TBL.n[i].x, TBL.n[i].y, 16, 4, 0, 172, 320, 12)) {
                        // good
                        show_result(2);
                        TBL.n[i].flag = 0;
                        add_bomb(TBL.n[i].pos, TBL.n[i].y);
                    } else if (HITCHK(TBL.n[i].x, TBL.n[i].y, 16, 4, 0, 170, 320, 16)) {
                        // bad
                        show_result(1);
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
                        show_result(0);
                    }
                }
            }
        }
    }
}

void draw_lane(int x, int y) {
    static unsigned char c[15] = {
        102, 13, 13, 102, 13, 13, 13, 102, 13, 13, 102, 13, 13, 13, 102
    };
    int i;
    for (i = 0; i < 15; i++) {
        vgs2_lineSP(i * 20 + x, 0, i * 20 + x, y - 4, c[i]);
    }
}

void draw_keyboard(int x, int y) {
    int i;
    int n = 0;
    vgs2_boxfSP(0, y, 320, 240, 3);
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
        add_note(KAERU[TBL.spos].pos + TBL.z, KAERU[TBL.spos].len);
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
    vgs2_boxSP(0, 176, 319, 179, 105);
    draw_result();
    draw_combo();
    draw_notes();
    draw_bomb();
    draw_keyboard(21, 180);
    return 0;
}

int vgs2_pause() {
    return 0;
}

void vgs2_term() {
}
