#include "psyq.h"

void SpuSetCommonAttr(SpuCommonAttr *attr) {
    u_short mode_l;
    u_short mode_r;
    u_short vol_l;
    u_short vol_r;
    u_long mask;
    long all;
    u_short cnt;

    vol_l = 0;
    vol_r = 0;
    mask = attr->mask;
    all = attr->mask == 0;

    if (all || (mask & SPU_COMMON_MVOLL)) {
        if (all || (mask & SPU_COMMON_MVOLMODEL)) {
            switch (attr->mvolmode.left) {
            case SPU_VOICE_LINEARIncN:
                mode_l = 0x8000;
                break;
            case SPU_VOICE_LINEARIncR:
                mode_l = 0x9000;
                break;
            case SPU_VOICE_LINEARDecN:
                mode_l = 0xA000;
                break;
            case SPU_VOICE_LINEARDecR:
                mode_l = 0xB000;
                break;
            case SPU_VOICE_EXPIncN:
                mode_l = 0xC000;
                break;
            case SPU_VOICE_EXPIncR:
                mode_l = 0xD000;
                break;
            case SPU_VOICE_EXPDec:
                mode_l = 0xE000;
                break;
            case SPU_VOICE_DIRECT:
                vol_l = attr->mvol.left;
                mode_l = 0;
                break;
            default:
                vol_l = attr->mvol.left;
                mode_l = 0;
                break;
            }
        } else {
            vol_l = attr->mvol.left;
            mode_l = 0;
        }
        if (mode_l != 0) {
            if (attr->mvol.left >= 0x80) {
                vol_l = 0x7F;
            } else if (attr->mvol.left < 0) {
                vol_l = 0;
            } else {
                vol_l = attr->mvol.left;
            }
        }
        vol_l &= 0x7FFF;
        D_8006EF24[0xC0] = vol_l | mode_l;
    }
    if (all || (mask & SPU_COMMON_MVOLR)) {
        if (all || (mask & SPU_COMMON_MVOLMODER)) {
            switch (attr->mvolmode.right) {
            case SPU_VOICE_LINEARIncN:
                mode_r = 0x8000;
                break;
            case SPU_VOICE_LINEARIncR:
                mode_r = 0x9000;
                break;
            case SPU_VOICE_LINEARDecN:
                mode_r = 0xA000;
                break;
            case SPU_VOICE_LINEARDecR:
                mode_r = 0xB000;
                break;
            case SPU_VOICE_EXPIncN:
                mode_r = 0xC000;
                break;
            case SPU_VOICE_EXPIncR:
                mode_r = 0xD000;
                break;
            case SPU_VOICE_EXPDec:
                mode_r = 0xE000;
                break;
            case SPU_VOICE_DIRECT:
                vol_r = attr->mvol.right;
                mode_r = 0;
                break;
            default:
                vol_r = attr->mvol.right;
                mode_r = 0;
                break;
            }
        } else {
            vol_r = attr->mvol.right;
            mode_r = 0;
        }
        if (mode_r != 0) {
            if (attr->mvol.right >= 0x80) {
                vol_r = 0x7F;
            } else if (attr->mvol.right < 0) {
                vol_r = 0;
            } else {
                vol_r = attr->mvol.right;
            }
        }
        vol_r &= 0x7FFF;
        D_8006EF24[0xC1] = vol_r | mode_r;
    }
    if (all || (mask & SPU_COMMON_CDVOLL)) {
        D_8006EF24[0xD8] = attr->cd.volume.left;
    }
    if (all || (mask & SPU_COMMON_CDVOLR)) {
        D_8006EF24[0xD9] = attr->cd.volume.right;
    }
    if (all || (mask & SPU_COMMON_EXTVOLL)) {
        D_8006EF24[0xDA] = attr->ext.volume.left;
    }
    if (all || (mask & SPU_COMMON_EXTVOLR)) {
        D_8006EF24[0xDB] = attr->ext.volume.right;
    }
    if (all || (mask & SPU_COMMON_CDREV)) {
        if (attr->cd.reverb == 0) {
            cnt = D_8006EF24[0xD5];
            cnt &= ~4;
            D_8006EF24[0xD5] = cnt;
        } else {
            cnt = D_8006EF24[0xD5];
            cnt |= 4;
            D_8006EF24[0xD5] = cnt;
        }
    }
    if (all || (mask & SPU_COMMON_CDMIX)) {
        if (attr->cd.mix == 0) {
            cnt = D_8006EF24[0xD5];
            cnt &= ~1;
            D_8006EF24[0xD5] = cnt;
        } else {
            cnt = D_8006EF24[0xD5];
            cnt |= 1;
            D_8006EF24[0xD5] = cnt;
        }
    }
    if (all || (mask & SPU_COMMON_EXTREV)) {
        if (attr->ext.reverb == 0) {
            cnt = D_8006EF24[0xD5];
            cnt &= ~8;
            D_8006EF24[0xD5] = cnt;
        } else {
            cnt = D_8006EF24[0xD5];
            cnt |= 8;
            D_8006EF24[0xD5] = cnt;
        }
    }
    if (all || (mask & SPU_COMMON_EXTMIX)) {
        if (attr->ext.mix == 0) {
            cnt = D_8006EF24[0xD5];
            cnt &= ~2;
            D_8006EF24[0xD5] = cnt;
        } else {
            cnt = D_8006EF24[0xD5];
            cnt |= 2;
            D_8006EF24[0xD5] = cnt;
        }
    }
}

OBJECT_END(1);
