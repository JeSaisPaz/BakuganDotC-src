// bdc 0x0886f694 BtlCalcVoiceBank
#include "bdc.h"

/* Returns the voice sound bank of a new battle unit, stored in `+0x5a0` by `BtlCreateBakugan`
   (via `BtlBakuganCalcVoiceBank`). Outside battle rule mode 1 (`g_scriptGlobalVars` entry 8)
   the bank follows the unit kind: 1/2/0x10 → 0x44, 3/4 → 0x45, 5/0x11 → 0x46, 6/0x12 → 0x47,
   7/0x13 → 0x48, 8/0x14 → 0x49, 9/10 → 0x4a, 0xb → 0x4b, 0xc → 0x4c, 0xd → 0x4d, 0xe → 0x4e,
   0xf → 0x4f, any other kind → 0x4e. In mode 1 the player gets 0x50 and other units a bank chosen
   by the stage number (entry 1): 6 → 0x4c, 8 → 0x4c (0x4f for kind 0xf), 9 → 0x4e (0x4b for kind
   0xb), 10/0xe → 0x4d, 0xd/0x18/0x25 → 0x4a, 0x10/0x12 → 0x4f, 0x24 → 0x50, any other stage →
   0x4e. Sound ids are `bank << 20 | index`. */
int BtlCalcVoiceBank(int kind, char isPlayer)
{
    if (g_scriptGlobalVars[8] != 1) {
        switch (kind) {
        case 1:
        case 2:
        case 0x10:
            return 0x44;
        case 3:
        case 4:
            return 0x45;
        case 5:
        case 0x11:
            return 0x46;
        case 6:
        case 0x12:
            return 0x47;
        case 7:
        case 0x13:
            return 0x48;
        case 8:
        case 0x14:
            return 0x49;
        case 9:
        case 10:
            return 0x4a;
        case 0xb:
            return 0x4b;
        case 0xc:
            return 0x4c;
        case 0xd:
            return 0x4d;
        case 0xe:
            return 0x4e;
        case 0xf:
            return 0x4f;
        default:
            return 0x4e;
        }
    }
    if (isPlayer != 0) {
        return 0x50;
    }
    switch (g_scriptGlobalVars[1]) {
    case 6:
        return 0x4c;
    case 8:
        return (kind == 0xf) ? 0x4f : 0x4c;
    case 9:
        return (kind == 0xb) ? 0x4b : 0x4e;
    case 10:
    case 0xe:
        return 0x4d;
    case 0xd:
    case 0x18:
    case 0x25:
        return 0x4a;
    case 0x10:
    case 0x12:
        return 0x4f;
    case 0x24:
        return 0x50;
    default:
        return 0x4e;
    }
}
