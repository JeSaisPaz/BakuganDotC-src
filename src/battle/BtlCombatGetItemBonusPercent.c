// bdc 0x08887e18 BtlCombatGetItemBonusPercent
#include "bdc.h"

/* Percentage points granted by an equipped boost item `itemId` (see `BtlCombatHasUpgrade`):
   1→30, 2→70, 3→50, 4→100, 7/9/0xb/0xd→50, 8/0xa/0xc/0xe→100, 0xf/0x11→30,
   0x10/0x12→70, 0x19→50, 0x1a→100, anything else 1.0. `combat` is unused. */
float BtlCombatGetItemBonusPercent(BtlCombatState *combat, s32 itemId)
{
    (void)combat;
    switch (itemId) {
    case 1:
    case 0xf:
    case 0x11:
        return 30.0f;
    case 2:
    case 0x10:
    case 0x12:
        return 70.0f;
    case 3:
    case 7:
    case 9:
    case 0xb:
    case 0xd:
    case 0x19:
        return 50.0f;
    case 4:
    case 8:
    case 10:
    case 0xc:
    case 0xe:
    case 0x1a:
        return 100.0f;
    default:
        return 1.0f;
    }
}
