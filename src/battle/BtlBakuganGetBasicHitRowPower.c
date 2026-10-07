// bdc 0x088872a4 BtlBakuganGetBasicHitRowPower
#include "bdc.h"

/* Returns the normal power `[0]` (sign-extended s16) of basic-hit row `row`
   (`BtlBakuganGetBasicHitRow`), or 20 when the row is missing. `BtlCombatCalcBaseDamage` reads
   rows 0x16/0x17/0x18 for the attack ids 0xb3/0xb4/0xb5..0xb7. */
s32 BtlBakuganGetBasicHitRowPower(BtlBakugan *self, s32 row)
{
    s16 *hit = BtlBakuganGetBasicHitRow(self, row);

    if (hit == NULL) {
        return 20;
    }
    return hit[0];
}
