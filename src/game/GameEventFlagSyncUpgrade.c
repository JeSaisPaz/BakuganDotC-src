// bdc 0x088f5220 GameEventFlagSyncUpgrade
#include "bdc.h"

/* Maps class-relative flags 3, 0xf..0x13 to Bakugan pair indices 3, 1, 5..8 and refreshes that
   pair's upgrades (`UiBakuganEvolve`). Called by `GameEventFlagSet`. */

void GameEventFlagSyncUpgrade(s16 index) {
    s32 pair = -1;

    switch (index) {
    case 3:
        pair = 3;
        break;
    case 0xf:
        pair = 1;
        break;
    case 0x10:
        pair = 5;
        break;
    case 0x11:
        pair = 6;
        break;
    case 0x12:
        pair = 7;
        break;
    case 0x13:
        pair = 8;
        break;
    }
    if (pair != -1) {
        UiBakuganEvolve((u8)pair);
    }
}
