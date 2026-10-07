// bdc 0x088871e0 BtlBakuganGetBasicHitPower
#include "bdc.h"

/* Power of the unit's current basic (combo) hit, from the row returned by
   `BtlBakuganGetBasicHitParams`: `row[0]` when `normal` is nonzero, else
   `(s32)(row[5] * 0.7f)` (truncated); 20 when there is no row. `BtlCombatCalcBaseDamage` passes
   `normal = (special == 0)`. */
s32 BtlBakuganGetBasicHitPower(BtlBakugan *self, s32 attackId, u8 normal)
{
    s16 *row = BtlBakuganGetBasicHitParams(self, attackId);

    if (row == NULL) {
        return 20;
    }
    if (normal != 0) {
        return row[0];
    }
    return (s32)((float)row[5] * 0.699999988f);
}
