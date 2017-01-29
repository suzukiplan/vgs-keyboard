//
//  vgs-keyboard.c
//  VGSKeyboard
//
//  Created by Yoji Suzuki on 2017/01/29.
//  Copyright © 2017年 SUZUKI PLAN. All rights reserved.
//

#include "vgs-keyboard.h"
#include "vgstone.h"

#define INIT_MAX 100
#define INIT_ENV_S 16
#define INIT_ENV_E 80

extern int KEY_FLAG[256];

struct _KeyStatus {
    int on;
    int pow;
    int max;
    short* tone;
    int envSi;
    int envEi;
    int envC;
    int cur;
    int note;
};

extern short* TONE1[85];
extern short* TONE2[85];
extern short* TONE3[85];
extern short* TONE4[85];

struct _KeyStatus KS[24] = {
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 48},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 49},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 50},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 51},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 52},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 53},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 54},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 55},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 56},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 57},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 58},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 59},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 60},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 61},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 62},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 63},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 64},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 65},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 66},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 67},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 68},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 69},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 70},
    {0, 0, INIT_MAX, NULL, INIT_ENV_S, INIT_ENV_E, 0, 0, 71}
};


void vgskey_exec(void* data, size_t size)
{
    short* s;
    int i, j, w, ww;
    size /= 2;
    for (i = 0; i < 24; i++) {
        // 初期値設定
        if (!KS[i].tone) {
            KS[i].tone = TONE2[KS[i].note];
        }
        // 現在のキー入力状態を保持
        KS[i].on = KEY_FLAG[KS[i].note];
        // キー毎の波形を算出して合成
        for (s = (short*)data, j = 0; j < size; j++) {
            // 現在の波形を算出 & 合成
            KS[i].cur++;
            if (KS[i].pow) {
                w = KS[i].tone[(1 + KS[i].cur) % KS[i].tone[0]];
                w *= KS[i].pow * 32;
                w /= KS[i].max;
                ww = s[j] + w;
                if (32767 < ww) ww = 32767;
                else if (ww < -32768) ww = -32768;
                s[j] = ww;
            }
            // keyOnの状態によって減衰 or 増幅
            KS[i].envC++;
            if (KS[i].on) {
                if (KS[i].envSi < KS[i].envC) {
                    KS[i].envC = 0;
                    KS[i].pow++;
                    if (KS[i].max < KS[i].pow) {
                        KS[i].pow = KS[i].max;
                    }
                }
            } else {
                if (KS[i].envEi < KS[i].envC) {
                    KS[i].envC = 0;
                    KS[i].pow--;
                    if (KS[i].pow < 0) {
                        KS[i].pow = 0;
                    }
                }
            }
        }
    }
}
