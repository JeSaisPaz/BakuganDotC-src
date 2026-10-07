// bdc 0x088870e4 BtlBakuganGetBasicHitTable
#include "bdc.h"

/* Returns the basic-hit parameter table of the unit's kind (`base.base.unk08`) from
   `g_btlBasicHitTables`; NULL for a NULL unit. The kind is converted to float and clamped:
   below 1.0 gives index 1, at most 32.0 gives the truncated kind, otherwise (also NaN) index 32.
   Rows are 14 bytes (7 x s16), indexed by `BtlBakuganGetBasicHitIndex`. */
s16 *BtlBakuganGetBasicHitTable(BtlBakugan *self)
{
    float kind;
    s32 index;

    if (self == NULL) {
        return NULL;
    }
    kind = (float)(s32)self->base.base.unk08;
    if (kind < 1.0f) {
        index = 1;
    } else if (kind <= 32.0f) {
        index = (s32)kind;
    } else {
        index = 32;
    }
    return g_btlBasicHitTables[index];
}
