#include "psyq.h"

extern u_short D_8006EFC0[24];
long _spu_FsetRXXa(long reg, u_long addr);
u_short _spu_note2pitch(int cen_note, int cen_fine, int note, int fine);

void SpuSetVoiceAttr(SpuVoiceAttr *attr) {
    volatile int i;
    volatile int n;
    int voice;
    int pos;
    u_long mask = attr->mask;
    int all = attr->mask == 0;
    u_short vol;
    u_short mode;
    u_short value;
    u_short bits;
    u_short adsr;

    for (voice = 0; voice < 24; voice++) {
        if (!(attr->voice & (1 << voice))) {
            continue;
        }
        pos = voice * 8;
        if (all || (mask & SPU_VOICE_PITCH)) {
            D_8006EF24[voice * 8 + 2] = attr->pitch;
        }
        if (all || (mask & SPU_VOICE_SAMPLE_NOTE)) {
            D_8006EFC0[voice] = attr->sample_note;
        }
        if (all || (mask & SPU_VOICE_NOTE)) {
            D_8006EF24[pos + 2] = _spu_note2pitch(D_8006EFC0[voice] >> 8, D_8006EFC0[voice] & 0xFF,
                                                  attr->note >> 8, attr->note & 0xFF);
        }
        if (all || (mask & SPU_VOICE_VOLL)) {
            vol = attr->volume.left & 0x7FFF;
            mode = 0;
            if (all || (mask & SPU_VOICE_VOLMODEL)) {
                switch ((short)attr->volmode.left) {
                case 1:
                    mode = 0x8000;
                    break;
                case 2:
                    mode = 0x9000;
                    break;
                case 3:
                    mode = 0xA000;
                    break;
                case 4:
                    mode = 0xB000;
                    break;
                case 5:
                    mode = 0xC000;
                    break;
                case 6:
                    mode = 0xD000;
                    break;
                case 7:
                    mode = 0xE000;
                    break;
                }
            }
            if (mode != 0) {
                if (attr->volume.left >= 0x80) {
                    vol = 0x7F;
                } else if (attr->volume.left < 0) {
                    vol = 0;
                }
            }
            D_8006EF24[pos + 0] = vol | mode;
        }
        if (all || (mask & SPU_VOICE_VOLR)) {
            vol = attr->volume.right & 0x7FFF;
            mode = 0;
            if (all || (mask & SPU_VOICE_VOLMODER)) {
                switch ((short)attr->volmode.right) {
                case 1:
                    mode = 0x8000;
                    break;
                case 2:
                    mode = 0x9000;
                    break;
                case 3:
                    mode = 0xA000;
                    break;
                case 4:
                    mode = 0xB000;
                    break;
                case 5:
                    mode = 0xC000;
                    break;
                case 6:
                    mode = 0xD000;
                    break;
                case 7:
                    mode = 0xE000;
                    break;
                }
            }
            if (mode != 0) {
                if (attr->volume.right >= 0x80) {
                    vol = 0x7F;
                } else if (attr->volume.right < 0) {
                    vol = 0;
                }
            }
            D_8006EF24[pos + 1] = vol | mode;
        }
        if (all || (mask & SPU_VOICE_WDSA)) {
            _spu_FsetRXXa(pos | 3, attr->addr);
        }
        if (all || (mask & SPU_VOICE_LSAX)) {
            _spu_FsetRXXa(pos | 7, attr->loop_addr);
        }
        if (all || (mask & SPU_VOICE_ADSR_ADSR1)) {
            D_8006EF24[pos + 4] = attr->adsr1;
        }
        if (all || (mask & SPU_VOICE_ADSR_ADSR2)) {
            D_8006EF24[pos + 5] = attr->adsr2;
        }
        if (all || (mask & SPU_VOICE_ADSR_AR)) {
            value = attr->ar;
            if (value >= 0x80) {
                value = 0x7F;
            }
            bits = 0;
            if (all || (mask & SPU_VOICE_ADSR_AMODE)) {
                if (attr->a_mode == 5) {
                    bits = 0x80;
                }
            }
            adsr = D_8006EF24[pos + 4];
            adsr &= 0xFF;
            D_8006EF24[pos + 4] = adsr | ((value | bits) << 8);
        }
        if (all || (mask & SPU_VOICE_ADSR_DR)) {
            value = attr->dr;
            if (value >= 0x10) {
                value = 0xF;
            }
            D_8006EF24[pos + 4] = (D_8006EF24[pos + 4] & 0xFF0F) | (value << 4);
        }
        if (all || (mask & SPU_VOICE_ADSR_SR)) {
            value = attr->sr;
            if (value >= 0x80) {
                value = 0x7F;
            }
            bits = 0x100;
            if (all || (mask & SPU_VOICE_ADSR_SMODE)) {
                switch (attr->s_mode) {
                case 1:
                    bits = 0;
                    break;
                case 5:
                    bits = 0x200;
                    break;
                case 7:
                    bits = 0x300;
                    break;
                }
            }
            adsr = D_8006EF24[pos + 5];
            adsr &= 0x3F;
            D_8006EF24[pos + 5] = adsr | ((value | bits) << 6);
        }
        if (all || (mask & SPU_VOICE_ADSR_RR)) {
            value = attr->rr;
            if (value >= 0x20) {
                value = 0x1F;
            }
            bits = 0;
            if (all || (mask & SPU_VOICE_ADSR_RMODE)) {
                switch (attr->r_mode) {
                case 3:
                    break;
                case 7:
                    bits = 0x20;
                    break;
                }
            }
            D_8006EF24[pos + 5] = (D_8006EF24[pos + 5] & 0xFFC0) | (value | bits);
        }
        if (all || (mask & SPU_VOICE_ADSR_SL)) {
            value = attr->sl;
            if (value >= 0x10) {
                value = 0xF;
            }
            adsr = D_8006EF24[pos + 4];
            adsr &= 0xFFF0;
            D_8006EF24[pos + 4] = adsr | value;
        }
    }
    n = 1;
    for (i = 0; i < 2; i = i + 1) {
        n = n * 13;
    }
}


OBJECT_END(2);
