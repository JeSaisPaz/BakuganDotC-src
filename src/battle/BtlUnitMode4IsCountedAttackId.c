// bdc 0x0885dafc BtlUnitMode4IsCountedAttackId
#include "bdc.h"

/* Returns 1 when `attackId` is one of the attacks the mode-4 unit counts in its attacker's
   statistics: ids 0x2c..0x2f (searched in a local table of four words) or 0xb5..0xb7; else 0.
   `unit` is unused. Used by `BtlUnitMode4OnHit`. */
s32 BtlUnitMode4IsCountedAttackId(void *unit, s32 attackId)
{
    s32 ids[4];
    s32 i;

    (void)unit;
    ids[0] = 0x2c;
    ids[1] = 0x2d;
    ids[2] = 0x2e;
    ids[3] = 0x2f;
    for (i = 0; i < 4; i++) {
        if (ids[i] == attackId) {
            return 1;
        }
    }
    if (attackId > 0xb4 && attackId < 0xb8) {
        return 1;
    }
    return 0;
}
