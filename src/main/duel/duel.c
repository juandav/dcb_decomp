#include "common.h"
#include "gte.h"
#include "game.h"

void func_80034260(void) {
    char buf[0x88];
    s32 mode;
    s32 i;
    s32 j;
    s32 id;
    s8 c;
    s8 *card;
    s32 over;
    Player *a;
    Player *b;

    while (DUEL->state < 0) {
        func_80014C08(D_800794F0);
    }
    DUEL->unk815 = 0;
    for (;;) {
        func_80033D08(1);
        func_801EAB4C();
        switch (DUEL->unk818) {
        case 0:
            D_801D83EC[0x9D] = 1;
            D_801D83EC[0x175] = 1;
            D_801D83D0.next = -1;
            D_801D83D0.next2 = -1;
            func_80014C08(0x1E);
            if (PLAYER(1)->unk178_17 == 1 && ((u8 *)D_8006E054)[4] == 0x8C) {
                for (i = 0, j = 0; i < 30; i++) {
                    if ((u32)func_80047B84(0, PLAYER(0)->cards[i].id) < 3) {
                        j = 1;
                    }
                    if ((u32)func_80048150(0, PLAYER(0)->cards[i].id) < 3) {
                        j = 1;
                    }
                }
                if (j) {
                    for (i = 0; i < 3; i++) {
                        id = func_800402CC(0);
                        if (id == -1) {
                            break;
                        }
                        for (j = 0; j < 29; j++) {
                            PLAYER(0)->unk17D[j] = PLAYER(0)->unk17D[j + 1];
                        }
                        PLAYER(0)->unk17D[29] = id;
                    }
                    func_800149B8(0, -1, 0, 0x800, func_80049EF8, 2, func_800148B0(), 0, 0);
                    func_80014C08(0x7FFFFFFF);
                }
            }
            DUEL->unk818++;
            break;
        case 1:
            DUEL->unk822 = 0;
            if (ME == 0) {
                D_801D83EC[0xC1] = 1;
                D_801D83EC[0x199] = 6;
            } else {
                D_801D83EC[0xC1] = 6;
                D_801D83EC[0x199] = 1;
            }
            D_801D83EC[0x9D] = 1;
            D_801D83EC[0x175] = 1;
            PLAYER(0)->unk178_2 = 3;
            PLAYER(1)->unk178_2 = 3;
            DUEL->unk80A = -1;
            DUEL->unk80E = -1;
            DUEL->unk81D = -1;
            DUEL->unk818++;
            break;
        case 2:
            DUEL->unk822 = 0;
            D_801D83D0.unk3 = 0;
            D_801D83D0.unk1 = PLAYER(ME)->unk178_17;
            D_801D83D0.next = 0;
            D_801D83D0.next2 = 0;
            func_80033D08(0x3C);
            while (1) {
            wait:
                if (D_801D83EC[0x9D] != 4) {
                    goto wait;
                }
                if (func_801EC570(ME) == -1) {
                    break;
                }
                func_80014C08(0x14);
                func_801FA780(ME);
            }
            DUEL->unk818++;
            break;
        case 3:
            DUEL->unk822 = 0;
            D_801D83D7 = 0;
            if (func_80040764(ME) >= 0) {
                DUEL->unk818 = 4;
            } else if (func_80040570(ME) != 0) {
                if (func_80040220(ME) == 0) {
                    D_801D83D4 = 2;
                    sprintf(buf, "There are no more Cards, so %s loses!", PLAYER(ME)->unk1CE);
                    func_80019EA4((u8 *)&D_801D8278, buf, 0);
                    func_800341EC();
                    DUEL->unk81E = ME ^ 1;
                    DUEL->unk818 = 0x26;
                } else {
                    D_801D83D4 = 1;
                    if (PLAYER(ME)->unk178_17 != 1) {
                        func_80019EA4((u8 *)&D_801D8278, "Redrawing Cards because there are\nno Digimon Cards.", 0);
                        func_800341EC();
                    }
                    DUEL->unk818 = 6;
                }
            } else {
                DUEL->unk818 = 4;
            }
            break;
        case 4:
            DUEL->unk822 = 0;
            D_801D83D0.next = 0;
            D_801D83D0.next2 = 0;
            if (func_80040220(ME) == 0) {
                DUEL->unk818 = 8;
            } else if (PLAYER(ME)->unk178_17 == 1) {
                DUEL->unk827 = ME;
                DUEL->unk816 = 1;
                func_80033E7C();
                if (DUEL->unk804 != 0) {
                    func_80033D08(0x1E);
                    DUEL->unk818 = 6;
                } else {
                    DUEL->unk818 = 8;
                }
            } else {
                DUEL->unk818++;
            }
            break;
        case 5:
            DUEL->unk822 = 1;
            D_801D83D0.next2 = 1;
            if (D_80089840[ME]->unkA & 0x10) {
                func_8002B498(0xA0);
                D_801D83D0.next = 3;
                D_801D83D0.next2 = 0;
                do {
                    func_80019EA4((u8 *)&D_801D8278, "This will discard all Cards.\nIs this OK?", 1);
                    func_800341EC();
                    switch (CHOICE) {
                    case 0:
                    case 2:
                        if (DUEL->unk81F != 0) {
                            func_801EA8B4(0x78, "Please press \"Yes\"!");
                        } else {
                            DUEL->unk818 = 4;
                        }
                        break;
                    case 1:
                        PLAYER(ME)->unk110 |= 0x20;
                        DUEL->unk818++;
                        break;
                    }
                } while (DUEL->unk818 == 5);
            } else if (D_80089840[ME]->unkA & 0x40) {
                func_8002B498(0xA0);
                DUEL->unk818 = 8;
            } else if (D_80089840[ME]->unkA & 0x80) {
                func_8002B498(0xA0);
                DUEL->unk818 = 7;
                DUEL->unk819 = 4;
                DUEL->unk81A = ME;
            }
            break;
        case 6:
            DUEL->unk822 = 0;
            D_801D83D0.next = 0x10;
            D_801D83D0.next2 = 0;
            func_801ECC58(ME);
            DUEL->unk818 = 3;
            break;
        case 7:
            DUEL->unk822 = 1;
            D_801D83D7 = 8;
            if (DUEL->unk819 != 0x19) {
                func_801EC4CC(DUEL->unk81A);
            }
            func_801EBACC(DUEL->unk81A, 0);
            func_80033D08(0x14);
            for (;;) {
                func_80033D08(1);
                func_801EBACC(DUEL->unk81A, 0);
                if (D_80089840[DUEL->unk81A]->unkA & 0x10) {
                    func_8002B498(0xA1);
                    if (DUEL->unk819 == 0x19) {
                        D_801D83EC[0x31] = 1;
                        D_801D83EC[0x109] = 1;
                    }
                    func_801EC528(DUEL->unk81A);
                    DUEL->unk81C = -1;
                    DUEL->unk818 = DUEL->unk819;
                    func_80033D08(0x14);
                    break;
                }
            }
            break;
        case 8:
            DUEL->unk822 = 0;
            if (func_80040764(ME) >= 0) {
                if (PLAYER(ME)->unk178_17 == 1) {
                    DUEL->unk818 = 0xB;
                } else {
                    DUEL->unk818 = 0xA;
                }
            } else {
                D_801D83D4 = 4;
                if (PLAYER(ME)->unk178_17 == 1) {
                    DUEL->unk827 = ME;
                    DUEL->unk816 = 2;
                    func_80033E7C();
                    if (DUEL->unk804 == -1) {
                        for (i = 0; i < 4; i++) {
                            if (PLAYER(ME)->unk1B9[i] != -1 && PLAYER(ME)->cards[(s8)(PLAYER(ME)->unk1B9[i] % 30)].state == 0) {
                                DUEL->unk804 = PLAYER(ME)->unk1B9[i];
                                break;
                            }
                        }
                    }
                    CUR_CARD = DUEL->unk804;
                    func_801EA558(CUR_CARD, ME);
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    i = func_80047B84(ME, PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].id);
                    if (i != -1) {
                        if (func_80048014(ME, func_80047A58(PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].id)) != -1) {
                            func_80033D08(0x3C);
                            func_801F6214(1, ME);
                            func_80040A48(ME, i);
                        }
                    }
                    func_80033D08(0x78);
                    DUEL->unk818 = 0xB;
                } else {
                    DUEL->unk818++;
                    func_801EC4CC(ME);
                }
            }
            break;
        case 9:
            DUEL->unk822 = 1;
            if (func_80040220(ME) != 0) {
                D_801D83D7 = 5;
            } else {
                D_801D83D7 = 3;
            }
            if (func_801EBACC(ME, 1) == 0) {
                if (PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].state == 0) {
                    func_8002B498(0xA0);
                    func_801EC528(ME);
                    func_801EA558(CUR_CARD, ME);
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    i = func_80047B84(ME, PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].id);
                    if (i != -1) {
                        if (func_80048014(ME, func_80047A58(PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].id)) != -1) {
                            D_801D83D4 = 5;
                            func_80019EA4((u8 *)&D_801D8278, "Do you want to Armor Digivolve?", 1);
                            func_800341EC();
                            switch (CHOICE) {
                            case 0:
                                if (DUEL->unk80A >= 0) {
                                    func_801EC84C(CUR_CARD, ME, DUEL->unk80A);
                                    DUEL->unk80A = -1;
                                    DUEL->unk818 = 8;
                                } else {
                                    DUEL->unk818 = 3;
                                }
                                break;
                            case 1:
                                func_801F6214(1, ME);
                                func_80040A48(ME, i);
                                PLAYER(ME)->unk110 |= 8;
                                func_801FA4E4(ME);
                                DUEL->unk818 = 0xB;
                                break;
                            case 2:
                                DUEL->unk818++;
                                break;
                            }
                        } else {
                            DUEL->unk818++;
                        }
                    } else {
                        DUEL->unk818++;
                    }
                }
            } else if ((D_80089840[ME]->unkA & 0x10) && func_80040220(ME) != 0) {
                func_8002B498(0xA1);
                func_801EC528(ME);
                DUEL->unk818 = 4;
            }
            break;
        case 10:
            DUEL->unk822 = 0;
            D_801D83D0.next2 = 0;
            D_801D83D0.next = 6;
            func_80019EA4((u8 *)&D_801D8278, "Is it OK to end the Preparation Phase?", 1);
            func_800341EC();
            switch (CHOICE) {
            case 0:
            case 2:
                if (DUEL->unk81F != 0) {
                    func_801EA8B4(0x78, "Press \"Yes\" to go to the next Phase!");
                } else {
                    if (DUEL->unk80A >= 0) {
                        func_801EC84C(CUR_CARD, ME, DUEL->unk80A);
                        DUEL->unk80A = -1;
                    }
                    DUEL->unk818 = 4;
                }
                break;
            case 1:
                if (DUEL->unk80A >= 0) {
                    func_801FA4E4(ME);
                }
                DUEL->unk818++;
                break;
            }
            break;
        case 11:
            DUEL->unk822 = 0;
            if (PLAYER(ME)->unk178_17 == 1) {
                DUEL->unk80A = -2;
                DUEL->unk80E = -1;
                DUEL->unk80C = -1;
                DUEL->unk827 = ME;
                DUEL->unk816 = 3;
                func_80033E7C();
                if (DUEL->unk804 == -1) {
                    DUEL->unk818 = 0xE;
                } else {
                    D_801D83D0.unk3 = 1;
                    D_801D83D0.next = 7;
                    CUR_CARD = DUEL->unk804;
                    DUEL->unk80A = func_801ECBCC(CUR_CARD, ME);
                    func_80033D08(0x78);
                    DUEL->unk818 = 0xE;
                }
            } else {
                DUEL->unk818++;
            }
            break;
        case 12:
            DUEL->unk822 = 0;
            DUEL->unk80A = -2;
            DUEL->unk80E = -1;
            DUEL->unk80C = -1;
            if (func_80040570(ME) != 0) {
                DUEL->unk818 = 0xE;
            } else if (func_80041214(ME) != 0) {
                func_801EC4CC(ME);
                DUEL->unk818++;
            } else {
                DUEL->unk818 = 0xE;
            }
            break;
        case 13:
            DUEL->unk822 = 1;
            D_801D83D0.unk3 = 1;
            D_801D83D0.next = 7;
            D_801D83D0.next2 = 7;
            if (func_801EBACC(ME, 2) == 0) {
                i = PLAYER(ME)->unk1B9[DUEL->unk81C];
                if (PLAYER(ME)->cards[i % 30].state == 0) {
                    func_8002B498(0xA0);
                    DUEL->unk80E = func_801ECBCC(CUR_CARD, ME);
                    DUEL->unk818++;
                }
            } else if (D_80089840[ME]->unkA & 0x20) {
                func_8002B498(0xA0);
                DUEL->unk818++;
            }
            if (DUEL->unk818 != 0xD) {
                func_801EC528(ME);
            }
            break;
        case 14:
            DUEL->unk822 = 0;
            if (PLAYER(ME)->unk178_17 == 1) {
                DUEL->unk827 = ME;
                DUEL->unk816 = 4;
                func_80033E7C();
                if (DUEL->unk804 == -1) {
                    DUEL->unk818 = 0x13;
                } else {
                    D_801D83D0.unk3 = 1;
                    D_801D83D0.next = 8;
                    D_801D83EC[ME * 0xD8 + 0x55] = 6;
                    func_80033D08(0x1E);
                    CUR_CARD = DUEL->unk804;
                    DUEL->unk80A = func_801ECB40(CUR_CARD, ME);
                    func_80033D08(0x1E);
                    DUEL->unk818 = 0x10;
                }
            } else if (func_800406BC(ME) != 0) {
                DUEL->unk80A = -1;
                DUEL->unk818 = 0x13;
            } else {
                DUEL->unk818++;
                func_801EC4CC(ME);
                D_801D83EC[ME * 0xD8 + 0x55] = 6;
                D_801D83D0.unk3 = 1;
                D_801D83D0.next = 8;
                D_801D83D0.next2 = 6;
                func_80033D08(0x1E);
            }
            break;
        case 15:
            DUEL->unk822 = 1;
            if (func_801EBACC(ME, 5) == 0) {
                i = PLAYER(ME)->unk1B9[DUEL->unk81C];
                if (PLAYER(ME)->cards[i % 30].state == 2) {
                    func_8002B498(0xA0);
                    DUEL->unk80A = func_801ECB40(CUR_CARD, ME);
                    if (func_801EA374(ME) != 0) {
                        func_80019EA4((u8 *)&D_801D8278, "This Digivolve Option has no Effect.\nDo you still want to use it?", 1);
                        func_800341EC();
                        switch (CHOICE) {
                        case 0:
                        case 2:
                            func_801EC8E0(ME, DUEL->unk80A);
                            DUEL->unk80A = -2;
                            break;
                        case 1:
                            func_801EC528(ME);
                            DUEL->unk818++;
                            break;
                        }
                    } else {
                        DUEL->unk818++;
                    }
                }
            } else if (D_80089840[ME]->unkA & 0x30) {
                DUEL->unk81C = -1;
                if (D_80089840[ME]->unkA & 0x20) {
                    func_8002B498(0xA0);
                    func_801EC528(ME);
                    D_801D83EC[ME * 0xD8 + 0x55] = 1;
                    DUEL->unk818 = 0x13;
                } else if (D_80089840[ME]->unkA & 0x10) {
                    func_8002B498(0xA1);
                    if (DUEL->unk80E >= 0) {
                        func_801ECA30(func_800411C4(ME), ME, DUEL->unk80E);
                        DUEL->unk80E = -1;
                    }
                    D_801D83EC[ME * 0xD8 + 0x55] = 1;
                    DUEL->unk818 = 0xB;
                }
            }
            break;
        case 16:
            DUEL->unk822 = 0;
            D_801D83D4 = 9;
            mode = 0;
            if (func_801EA374(ME) == 0) {
                card = PLAYER(ME)->cards[func_80041340(ME) % 30].card;
                switch (card[0x1A]) {
                case 4:
                    func_80019EA4((u8 *)&D_801D8278, "Current Digimon will be discarded,\ndo you still want to \"Digi-devolve\"?", 1);
                    if (PLAYER(ME)->unk178_17 != 1) {
                        func_800341EC();
                    } else {
                        D_801D831D = 1;
                    }
                    switch (D_801D831D) {
                    case 0:
                    case 2:
                        mode = 2;
                        func_801EC8E0(ME, DUEL->unk80A);
                        DUEL->unk80A = -2;
                        D_801D83EC[ME * 0xD8 + 0x55] = 1;
                        break;
                    case 1:
                        mode = 3;
                        func_801F6214(9, ME);
                        func_801EC608(func_80040764(ME), ME);
                        PLAYER(ME)->unk178_15 = 0;
                        func_8004080C(func_80040764(ME), ME);
                        PLAYER(ME)->unk11C[0] *= 2;
                        func_80033D08(0x14);
                        break;
                    }
                    break;
                case 7:
                    func_80019EA4((u8 *)&D_801D8278, "Your Digimon's Level will become *e3,\ndo you still want to \"Armor Digi-devolve\"?", 1);
                    if (PLAYER(ME)->unk178_17 != 1) {
                        func_800341EC();
                    } else {
                        D_801D831D = 1;
                    }
                    switch (D_801D831D) {
                    case 0:
                    case 2:
                        mode = 2;
                        func_801EC8E0(ME, DUEL->unk80A);
                        DUEL->unk80A = -2;
                        D_801D83EC[ME * 0xD8 + 0x55] = 1;
                        break;
                    case 1:
                        mode = 3;
                        func_801F6214(8, ME);
                        func_80040D88(ME, func_80048150(ME, PLAYER(ME)->cards[func_80040764(ME) % 30].id));
                        break;
                    }
                    break;
                default:
                    mode = 1;
                    break;
                }
            }
            if (mode == 0 || mode == 3) {
                DUEL->unk80C = DUEL->unk80A;
                i = func_80041408(ME);
                D_801D833C[i * 0x24 + 0x22] = 8;
                func_800400B4(i, ME);
                D_801D83EC[ME * 0xD8 + 0x55] = 1;
                func_801EC528(ME);
            }
            switch (mode) {
            case 0:
                DUEL->unk818 = 0x13;
                break;
            case 1:
                DUEL->unk818 = 0x11;
                break;
            case 2:
                DUEL->unk818 = 0xE;
                break;
            case 3:
                PLAYER(ME)->unk110 |= 0x40000000;
                DUEL->unk818 = 0x17;
                break;
            }
            if (PLAYER(ME)->unk178_17 == 1) {
                func_80033D08(0x3C);
            }
            break;
        case 17:
            DUEL->unk822 = 0;
            DUEL->unk818 = 0x12;
            break;
        case 18:
            DUEL->unk822 = 1;
            if (PLAYER(ME)->unk178_17 == 1) {
                DUEL->unk827 = ME;
                DUEL->unk816 = 5;
                func_80033E7C();
                if (DUEL->unk804 == -1) {
                    DUEL->unk818 = 0x17;
                    i = 0;
                    func_801EC528(ME);
                } else {
                    func_80033D08(0x3C);
                    CUR_CARD = DUEL->unk804;
                    i = 0;
                }
            } else {
                D_801D83D7 = 6;
                i = func_801EBACC(ME, 6);
                if (i != 0) {
                    if (D_80089840[ME]->unkA & 0x20) {
                        func_8002B498(0xA0);
                        if (DUEL->unk80A >= 0) {
                            func_801EC8E0(ME, DUEL->unk80A);
                            DUEL->unk80A = -2;
                        }
                        DUEL->unk818 = 0x13;
                        func_801EC528(ME);
                        D_801D83EC[ME * 0xD8 + 0x55] = 1;
                    } else if (D_80089840[ME]->unkA & 0x10) {
                        func_8002B498(0xA1);
                        if (DUEL->unk80A >= 0) {
                            func_801EC8E0(ME, DUEL->unk80A);
                            DUEL->unk80A = -2;
                        }
                        DUEL->unk818 = 0xE;
                        func_801EC528(ME);
                        D_801D83EC[ME * 0xD8 + 0x55] = 1;
                    }
                }
            }
            if (i == 0 && func_801E9F5C(CUR_CARD, ME) == 0) {
                func_801EC528(ME);
                card = PLAYER(ME)->cards[func_80041340(ME) % 30].card;
                switch (card[0x1A]) {
                case 0:
                    func_801F6214(3, ME);
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    break;
                case 1:
                    func_801F6214(4, ME);
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    break;
                case 2:
                    func_801F6214(5, ME);
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    break;
                case 3:
                    func_801F6214(7, ME);
                    func_801EC608(func_80040764(ME), ME);
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    break;
                case 5:
                    func_801F6214(2, ME);
                    func_80040D88(ME, func_80048150(ME, PLAYER(ME)->cards[func_80040764(ME) % 30].id));
                    while (func_80040764(ME) != -1) {
                        func_801EC608(func_80040764(ME), ME);
                        func_80033D08(0x14);
                    }
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    break;
                case 6:
                    func_801F6214(6, ME);
                    func_80040D88(ME, func_80048150(ME, PLAYER(ME)->cards[func_80040764(ME) % 30].id));
                    func_801EC608(func_80040764(ME), ME);
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    break;
                }
                PLAYER(ME)->unk110 |= 8;
                func_801FA4E4(ME);
                DUEL->unk818 = 0x17;
            }
            if (DUEL->unk818 != 0x12 && DUEL->unk818 != 0xE && DUEL->unk818 != 0x13) {
                i = func_80041408(ME);
                D_801D833C[i * 0x24 + 0x22] = 8;
                func_800400B4(i, ME);
                D_801D83EC[ME * 0xD8 + 0x55] = 1;
                func_80033D08(0x14);
                PLAYER(ME)->unk110 |= 0x40000000;
            }
            break;
        case 19:
            DUEL->unk822 = 0;
            if (PLAYER(ME)->unk178_17 == 1) {
                DUEL->unk827 = ME;
                DUEL->unk816 = 5;
                func_80033E7C();
                if (DUEL->unk804 == -1) {
                    DUEL->unk818 = 0x17;
                } else {
                    D_801D83D0.unk3 = 1;
                    D_801D83D0.next = 9;
                    func_801F6214(0, ME);
                    CUR_CARD = DUEL->unk804;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                }
            } else if (func_80040570(ME) != 0) {
                DUEL->unk818 = 0x16;
            } else if (func_801EA374(ME) != 0) {
                DUEL->unk818 = 0x16;
            } else {
                DUEL->unk818++;
            }
            break;
        case 20:
            DUEL->unk822 = 0;
            func_801EC4CC(ME);
            DUEL->unk818++;
            break;
        case 21:
            DUEL->unk822 = 1;
            D_801D83D0.unk3 = 1;
            D_801D83D0.next = 9;
            if (func_801EBACC(ME, 3) == 0) {
                if (func_801E9F5C(PLAYER(ME)->unk1B9[DUEL->unk81C], ME) == 0) {
                    func_801EC528(ME);
                    func_801F6214(0, ME);
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    PLAYER(ME)->unk110 |= 8;
                    func_801FA4E4(ME);
                }
            } else if (D_80089840[ME]->unkA & 0x20) {
                func_8002B498(0xA0);
                DUEL->unk818++;
                func_801EC528(ME);
            } else if (D_80089840[ME]->unkA & 0x10) {
                func_8002B498(0xA1);
                func_801ECD68();
                func_801EC528(ME);
            }
            break;
        case 22:
            DUEL->unk822 = 0;
            D_801D83D0.unk3 = 1;
            D_801D83D0.next2 = 0;
            D_801D83D0.next = 0xA;
            D_801D83EC[ME * 0xD8 + 0x55] = 1;
            func_80019EA4((u8 *)&D_801D8278, "Is it OK to end the Digivolve Phase?", 1);
            func_800341EC();
            switch (CHOICE) {
            case 0:
            case 2:
                if (DUEL->unk81F != 0) {
                    func_801EA8B4(0x78, "Choose \"Yes\" to go to next Phase!");
                } else if (DUEL->unk80A == -2) {
                    DUEL->unk818 = 0xE;
                } else {
                    func_801ECD68();
                }
                break;
            case 1:
                DUEL->unk818++;
                break;
            }
            break;
        case 23:
            DUEL->unk822 = 0;
            PLAYER(ME)->unk178_30 = 1;
            PLAYER(ME)->unk114 = PLAYER(ME)->cards[func_80040764(ME) % 30].card;
            if (func_80040764(OPP) == -1) {
                if (PLAYER(ME)->unk178_17 != 1) {
                    D_801D83D0.unk3 = 2;
                    D_801D83D0.next = 0xB;
                    sprintf(buf, "Since %s has no Digimon,\nthere is no Battle Phase.", PLAYER(OPP)->unk1CE);
                    func_80019EA4((u8 *)&D_801D8278, buf, 0);
                    func_800341EC();
                }
                DUEL->unk818 = 0x25;
            } else {
                DUEL->unk80A = -1;
                DUEL->unk80E = -1;
                DUEL->unk818++;
            }
            break;
        case 24:
            DUEL->unk822 = 0;
            D_801D83D0.unk3 = 2;
            D_801D83D0.next = 0xC;
            for (i = 0; i < 2; i++) {
                if (PLAYER(i)->unk178_17 == 1) {
                    DUEL->unk827 = i;
                    if (DUEL->unk81F != 0) {
                        DUEL->unk816 = 0;
                    } else {
                        DUEL->unk816 = 6;
                    }
                }
            }
            D_801D83EC[0x31] = 1;
            D_801D83EC[0x109] = 1;
            func_80014C08(0x1E);
            DUEL->unk818++;
            break;
        case 25:
            DUEL->unk822 = 1;
            D_801D83D7 = 2;
            for (i = 0; i < 2; i++) {
                if (PLAYER(i)->unk178_17 == 1) {
                    if (DUEL->unk816 == 0 && PLAYER(i)->unk178_2 == 3) {
                        func_8002B498(0xA0);
                        PLAYER(i)->unk178_2 = DUEL->unk804;
                    }
                } else if (PLAYER(i)->unk178_2 == 3) {
                    if (D_80089840[i]->unkA & 0x20) {
                        func_8002B498(0xA0);
                        PLAYER(i)->unk178_2 = 0;
                    } else if (D_80089840[i]->unkA & 0x10) {
                        func_8002B498(0xA0);
                        PLAYER(i)->unk178_2 = 1;
                    } else if (D_80089840[i]->unkA & 0x40) {
                        func_8002B498(0xA0);
                        PLAYER(i)->unk178_2 = 2;
                    } else if (D_80089840[i]->unkA & 0x80) {
                        func_8002B498(0xA0);
                        func_801EC4CC(i);
                        D_801D83EC[0x31] = 4;
                        D_801D83EC[0x109] = 4;
                        DUEL->unk818 = 7;
                        DUEL->unk819 = 0x19;
                        DUEL->unk81A = i;
                        break;
                    }
                }
            }
            if (PLAYER(0)->unk178_2 != 3 && PLAYER(1)->unk178_2 != 3) {
                func_80033D08(0x78);
                D_801D83EC[0x31] = 4;
                D_801D83EC[0x109] = 4;
                for (i = 0; i < 2; i++) {
                    PLAYER(i)->unk178_0 = PLAYER(i)->unk178_2;
                    if (((Unk8006E050 *)D_8006E050)[i].unk36[PLAYER(i)->unk178_0] != 0xFFFF) {
                        ((Unk8006E050 *)D_8006E050)[i].unk36[PLAYER(i)->unk178_0]++;
                    }
                }
                func_80033D08(0x78);
                DUEL->unk818++;
            }
            break;
        case 26:
            DUEL->unk822 = 0;
            D_801D83D0.next = 0xD;
            D_801D83D0.unk1 = PLAYER(OPP)->unk178_17;
            D_801D83EC[OPP * 0xD8 + 0x55] = 6;
            func_80033D08(0x1E);
            if (PLAYER(OPP)->unk178_17 == 1) {
                D_801D83D0.next2 = 0;
                DUEL->unk827 = OPP;
                DUEL->unk816 = 7;
                func_80033E7C();
                if (DUEL->unk804 == -2) {
                    func_80033D08(0x3C);
                    func_801ECAC4(OPP);
                    func_80033D08(0x78);
                } else if (DUEL->unk804 != -1) {
                    if (PLAYER(OPP)->cards[DUEL->unk804 % 30].card[2] < 2) {
                        func_80033D08(0x3C);
                        DUEL->unk80A = func_801ECB40(DUEL->unk804, OPP);
                        func_80033D08(0x78);
                    }
                }
                DUEL->unk818 = 0x1D;
            } else {
                DUEL->unk80A = -1;
                if (func_80040468(OPP) == 4 && func_80040220(OPP) == 0) {
                    func_80019EA4((u8 *)&D_801D8278, "You have no Cards left, so\nyou can't use any Support Cards!", 0);
                    func_8001A164(&D_801D8278, PLAYER(OPP)->unk178_17 & 1);
                    DUEL->unk818 = 0x1D;
                } else {
                    func_801EC4CC(OPP);
                    DUEL->unk818++;
                }
            }
            break;
        case 27:
            DUEL->unk822 = 1;
            D_801D83D7 = 7;
            if (func_801EBACC(OPP, 4) == 0) {
                i = PLAYER(OPP)->unk1B9[DUEL->unk81C];
                c = DUEL->unk81C;
                if (c == 4) {
                    DUEL->unk80A = c;
                    func_801ECAC4(OPP);
                } else {
                    if (PLAYER(OPP)->cards[i % 30].state == 2) {
                        break;
                    }
                    DUEL->unk80A = func_801ECB40(CUR_CARD, OPP);
                }
                func_8002B498(0xA0);
                func_80019EA4((u8 *)&D_801D8278, "Do you want to use this Support Card?", 1);
                DUEL->unk818++;
            } else if (D_80089840[OPP]->unkA & 0x20) {
                func_8002B498(0xA0);
                func_80019EA4((u8 *)&D_801D8278, "You're not using any Support Card.\nIs this OK?", 1);
                DUEL->unk818++;
            }
            if (DUEL->unk818 != 0x1B) {
                func_801EC528(OPP);
            }
            break;
        case 28:
            DUEL->unk822 = 0;
            D_801D83D7 = 0;
            func_8001A164(&D_801D8278, PLAYER(OPP)->unk178_17 & 1);
            switch (CHOICE) {
            case 0:
            case 2:
                if (DUEL->unk81F != 0) {
                    func_801EA8B4(0x78, "Please choose \"Yes\"!");
                    func_80019EA4((u8 *)&D_801D8278, "Do you want to use this Support Card?", 1);
                } else {
                    if (DUEL->unk80A >= 0) {
                        func_801EC8E0(OPP, DUEL->unk80A);
                    }
                    DUEL->unk818 = 0x1A;
                }
                break;
            case 1:
                DUEL->unk818++;
                break;
            }
            break;
        case 29:
            DUEL->unk822 = 0;
            D_801D83D0.next = 0xE;
            D_801D83D0.unk1 = PLAYER(ME)->unk178_17;
            D_801D83EC[ME * 0xD8 + 0x55] = 6;
            func_80033D08(0x1E);
            if (PLAYER(ME)->unk178_17 == 1) {
                D_801D83D0.next2 = 0;
                DUEL->unk827 = ME;
                DUEL->unk816 = 7;
                func_80033E7C();
                if (DUEL->unk804 == -2) {
                    func_80033D08(0x3C);
                    func_801ECAC4(ME);
                    func_80033D08(0x78);
                } else if (DUEL->unk804 != -1) {
                    if (PLAYER(ME)->cards[DUEL->unk804 % 30].card[2] < 2) {
                        func_80033D08(0x3C);
                        DUEL->unk80A = func_801ECB40(DUEL->unk804, ME);
                        func_80033D08(0x78);
                    }
                }
                DUEL->unk818 = 0x20;
            } else {
                DUEL->unk80A = -1;
                if (func_80040468(ME) == 4 && func_80040220(ME) == 0) {
                    func_80019EA4((u8 *)&D_801D8278, "You have no Cards left, so\nyou can't use any Support Cards!", 0);
                    func_8001A164(&D_801D8278, PLAYER(ME)->unk178_17 & 1);
                    DUEL->unk818 = 0x20;
                } else {
                    func_801EC4CC(ME);
                    DUEL->unk818++;
                }
            }
            break;
        case 30:
            DUEL->unk822 = 1;
            D_801D83D7 = 7;
            if (func_801EBACC(ME, 4) == 0) {
                i = PLAYER(ME)->unk1B9[DUEL->unk81C];
                c = DUEL->unk81C;
                if (c == 4) {
                    DUEL->unk80A = c;
                    func_801ECAC4(ME);
                } else {
                    if (PLAYER(ME)->cards[i % 30].state == 2) {
                        break;
                    }
                    DUEL->unk80A = func_801ECB40(CUR_CARD, ME);
                }
                func_8002B498(0xA0);
                func_80019EA4((u8 *)&D_801D8278, "Do you want to use this Support Card?", 1);
                DUEL->unk818++;
            } else if (D_80089840[ME]->unkA & 0x20) {
                func_8002B498(0xA0);
                func_80019EA4((u8 *)&D_801D8278, "You're not using any Support Card.\nIs this OK?", 1);
                DUEL->unk818++;
            }
            if (DUEL->unk818 != 0x1E) {
                func_801EC528(ME);
            }
            break;
        case 31:
            DUEL->unk822 = 0;
            D_801D83D7 = 0;
            func_800341EC();
            switch (CHOICE) {
            case 0:
            case 2:
                if (DUEL->unk81F != 0) {
                    func_801EA8B4(0x78, "Please choose \"Yes\"!");
                    func_80019EA4((u8 *)&D_801D8278, "Do you want to use this Support Card?", 1);
                } else {
                    if (DUEL->unk80A >= 0) {
                        func_801EC8E0(ME, DUEL->unk80A);
                    }
                    DUEL->unk818 = 0x1D;
                }
                break;
            case 1:
                DUEL->unk818++;
                break;
            }
            break;
        case 32:
            DUEL->unk822 = 0;
            D_801D83D0.next2 = 0;
            D_801D83D0.next = 0xF;
            func_80033D08(0x3C);
            if (((Unk8006E050 *)D_8006E050)->unk20_3) {
                D_801D8330 = 0x20;
                func_8001683C((s32)func_80033F34);
                while (D_801D8330 != 0) {
                    func_80014C08(D_800794F0);
                }
                func_80014C08(0x14);
            }
            func_801E6AA4(0);
            DUEL->unk818++;
            break;
        case 33:
            DUEL->unk822 = 0;
            D_801D83D4 = 0x11;
            DUEL->unk818++;
            break;
        case 34:
            DUEL->unk822 = 0;
            DUEL->unk81C = -1;
            if (!((Unk8006E050 *)D_8006E050)->unk20_3) {
                func_80014C08(0x3C);
                DUEL->state = 1;
                func_80014C08(2);
                DUEL->unk83C = 1;
                func_8002E26C();
                DUEL->unk83C = 0;
                func_80014C08(2);
                ((Unk800794F8 *)&D_800794F8)->unk54 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk56 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk58 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk7C = 0;
                ((Unk800794F8 *)&D_800794F8)->unk80 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk84 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk8E = 0;
                ((Unk800794F8 *)&D_800794F8)->unk90 = 0x1C0;
                ((Unk800794F8 *)&D_800794F8)->unk92 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk94 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk8C = -1;
                ((Unk800794F8 *)&D_800794F8)->unk74 = 1;
                func_80014C08(2);
                DUEL->state = 6;
            }
            DUEL->unk818++;
            break;
        case 35:
            DUEL->unk822 = 0;
            over = 0;
            if (((Unk8006E050 *)D_8006E050)->unk20_3) {
                a = DUEL->unk50;
                b = DUEL->unk54;
                j = a->unk178_17 & 1;
                func_80014C08(0x14);
                if (a->unk178_11 && !b->unk178_6) {
                    func_801F6268(0x1B, j);
                    a->unk11C[0] = 10;
                }
                if (b->unk162 == 0) {
                    func_801F6268(0x1C, j);
                } else if (a->unk178_12) {
                    func_801F6268(0x1A, j);
                    i = a->unk11C[0] + a->unk164;
                    func_80039354(j, i, 0);
                    if (i > 9990) {
                        i = 9990;
                    }
                    a->unk11C[0] = i;
                    if (i != 0 && i % 1110 == 0) {
                        func_801FB444(j, 0x1A);
                    }
                } else if (!a->unk178_11) {
                    func_801F6268(0x18, j);
                }
                i = b->unk11C[0] - b->unk162;
                if (b->unk162 != 0) {
                    func_80039354(j ^ 1, i, 0);
                    if (b->unk162 != 0 && b->unk162 % 1110 == 0) {
                        func_801FB444(j, 0x19);
                    }
                }
                if (i == 0 && a->unk17C == 2) {
                    func_801FB444(j, 0x13);
                }
                if (i < 0) {
                    i = 0;
                }
                b->unk11C[0] = i;
                if (i != 0 && i % 1110 == 0 && b->unk162 != 0) {
                    func_801FB444(j ^ 1, 0x1A);
                }
                func_8003917C();
                if (func_801ECF0C(j ^ 1) != 0) {
                    over = 1;
                }
                func_80014C08(0x14);
                if (b->unk15A != 0) {
                    if (b->unk178_11 && a->unk162 != 0) {
                        func_801F6268(0x1B, j ^ 1);
                        b->unk11C[0] = 10;
                    }
                    if (a->unk162 == 0) {
                        func_801F6268(0x1C, j ^ 1);
                    } else if (b->unk178_6 || b->unk178_12) {
                        if (b->unk178_6) {
                            func_801F6268(0x19, j ^ 1);
                        }
                        if (b->unk178_12) {
                            func_801F6268(0x1A, j ^ 1);
                            i = b->unk11C[0] + b->unk164;
                            func_80039354(j ^ 1, i, 0);
                            if (i > 9990) {
                                i = 9990;
                            }
                            b->unk11C[0] = i;
                            if (i != 0 && i % 1110 == 0) {
                                func_801FB444(j ^ 1, 0x1A);
                            }
                        }
                    } else if (!b->unk178_11) {
                        func_801F6268(0x18, j ^ 1);
                    }
                    i = a->unk11C[0] - a->unk162;
                    if (a->unk162 != 0) {
                        func_80039354(j, i, 0);
                        if (a->unk162 != 0 && a->unk162 % 1110 == 0) {
                            func_801FB444(j ^ 1, 0x19);
                        }
                    }
                    if (i == 0 && b->unk17C == 2) {
                        func_801FB444(j ^ 1, 0x13);
                    }
                    if (i < 0) {
                        i = 0;
                    }
                    a->unk11C[0] = i;
                    if (i != 0 && i % 1110 == 0 && a->unk162 != 0) {
                        func_801FB444(j, 0x1A);
                    }
                    func_8003917C();
                    if (func_801ECF0C(j) != 0) {
                        over = 1;
                    }
                }
                func_80016878((s32)func_80033F34);
                if (over) {
                    DUEL->unk818++;
                } else {
                    DUEL->unk818 = 0x25;
                }
            } else {
                func_80014C08(0x3C);
                PLAYER(0)->unk11C[0] = PLAYER(0)->unk15A;
                PLAYER(1)->unk11C[0] = PLAYER(1)->unk15A;
                if (PLAYER(ME)->unk11C[0] == PLAYER(ME)->unk126[0] && PLAYER(OPP)->unk11C[0] == PLAYER(OPP)->unk126[0]) {
                    if (func_801ECF0C(ME) != 0) {
                        DUEL->unk818++;
                    } else if (func_801ECF0C(OPP) != 0) {
                        DUEL->unk818++;
                    } else {
                        DUEL->unk818 = 0x25;
                    }
                }
            }
            break;
        case 36:
            DUEL->unk822 = 0;
            DUEL->unk818++;
            if (PLAYER(DUEL->unk81E)->unk17C == 2 && PLAYER(DUEL->unk81E ^ 1)->unk17C == 0) {
                PLAYER(DUEL->unk81E ^ 1)->unk110 |= 0x80;
                PLAYER(DUEL->unk81E)->unk110 |= 0x100;
            }
            if (func_80048150(DUEL->unk81E, PLAYER(DUEL->unk81E)->cards[func_80040764(DUEL->unk81E) % 30].id) >= 0) {
                func_801FB444(DUEL->unk81E, 0xA);
                PLAYER(DUEL->unk81E)->unk110 |= 0x4000;
            } else if (func_80047B84(DUEL->unk81E, PLAYER(DUEL->unk81E)->cards[func_80040764(DUEL->unk81E) % 30].id) >= 0) {
                func_801FB444(DUEL->unk81E, 0xA);
                PLAYER(DUEL->unk81E)->unk110 |= 0x4000;
            }
            if (PLAYER(DUEL->unk81E)->unk17C == 3) {
                if (PLAYER(DUEL->unk81E)->unk178_31) {
                    PLAYER(DUEL->unk81E)->unk110 |= 0x20000;
                    PLAYER(DUEL->unk81E ^ 1)->unk110 |= 0x40000;
                }
                sprintf(buf, "%d Wins, %d Losses-%s WINS!", PLAYER(DUEL->unk81E)->unk17C, PLAYER(DUEL->unk81E ^ 1)->unk17C, PLAYER(DUEL->unk81E)->unk1CE);
                func_80019EA4((u8 *)&D_801D8278, buf, 0);
                func_800341EC();
                DUEL->unk818 = 0x26;
            } else if (func_80040764(DUEL->unk81E ^ 1) == -1 && func_80040570(DUEL->unk81E ^ 1) != 0 && func_80040220(DUEL->unk81E ^ 1) == 0) {
                sprintf(buf, "Since %s has no more Digimon,\nthe winner is %s!", PLAYER(DUEL->unk81E ^ 1)->unk1CE, PLAYER(DUEL->unk81E)->unk1CE);
                func_80019EA4((u8 *)&D_801D8278, buf, 0);
                func_800341EC();
                DUEL->unk818 = 0x26;
            }
            break;
        case 37:
            DUEL->unk822 = 0;
            D_801D83D1 = PLAYER(ME)->unk178_17;
            DUEL->unk818++;
            for (i = 0; i < 2; i++) {
                PLAYER(i)->unk11C[1] = PLAYER(i)->unk15C[0];
                PLAYER(i)->unk11C[2] = PLAYER(i)->unk15C[1];
                PLAYER(i)->unk11C[3] = PLAYER(i)->unk15C[2];
                j = func_80041408(i);
                if (j != -1) {
                    D_801D833C[j * 0x24 + 0x22] = 8;
                    func_800400B4(j, i);
                }
                D_801D83EC[0x55] = 1;
                D_801D83EC[0x12D] = 1;
            }
            DUEL->unk817 ^= 1;
            DUEL->unk818 = 1;
            break;
        case 38:
            DUEL->unk822 = 0;
            DUEL->unk818++;
        case 39:
            DUEL->unk822 = 0;
            break;
        }
    }
}

INCLUDE_ASM("asm/main/nonmatchings/duel/duel", func_80038F68);
