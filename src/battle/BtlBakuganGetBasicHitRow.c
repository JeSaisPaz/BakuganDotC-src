// bdc 0x08887238 BtlBakuganGetBasicHitRow
#include "bdc.h"

/* Returns row `row` (clamped 0..0x18) of the unit's basic-hit table
   (`BtlBakuganGetBasicHitTable`; rows of 7 s16), or NULL when there is no table or
   `row == -1`. Like `BtlBakuganGetBasicHitParams` but with an explicit row. */
s16 *BtlBakuganGetBasicHitRow(BtlBakugan *self, s32 row)
{
    s16 *table = BtlBakuganGetBasicHitTable(self);

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
