// bdc 0x08887160 BtlBakuganGetBasicHitParams
#include "bdc.h"

/* Returns the basic-hit row (7 s16 values, 14 bytes) for the unit's current combo step: row
   `BtlBakuganGetBasicHitIndex``(self, attackId)`, clamped to 0..0x18, of the unit's
   `BtlBakuganGetBasicHitTable`; NULL when there is no table or the index is -1. */
s16 *BtlBakuganGetBasicHitParams(BtlBakugan *self, s32 attackId)
{
    s16 *table = BtlBakuganGetBasicHitTable(self);
    s32 row = BtlBakuganGetBasicHitIndex(self, attackId);

    if (table == NULL || row == -1) {
        return NULL;
    }
    if (row < 0) {
        row = 0;
    } else if (row > 0x18) {
        row = 0x18;
    }
    return &table[row * 7];
}
