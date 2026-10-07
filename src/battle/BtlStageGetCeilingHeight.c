// bdc 0x0889d748 BtlStageGetCeilingHeight
#include "bdc.h"

/* Returns the current arena's (`g_btlArenaIndex`) ceiling height: 1250.0 by default, 1000.0
   for arenas 8..0xb and 0x25/0x27, 1050.0 for 0x18/0x1b, 750.0 for 0x24/0x26. Units cannot fly
   through it: `BtlBakuganApplyVelocity` scales an upward velocity by `(ceiling - y) / 100` once
   a unit is within 100 units below it. */
float BtlStageGetCeilingHeight(void)
{
    switch (g_btlArenaIndex) {
    case 8:
    case 9:
    case 10:
    case 0xb:
    case 0x25:
    case 0x27:
        return 1000.0f;
    case 0x18:
    case 0x1b:
        return 1050.0f;
    case 0x24:
    case 0x26:
        return 750.0f;
    default:
        return 1250.0f;
    }
}
