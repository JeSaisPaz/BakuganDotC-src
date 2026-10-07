// bdc 0x088631e0 BtlBakuganDecayComboDamageScale
#include "bdc.h"

/* Records a landed hit on the attacker (called by `BtlCombatTakeHit`): while `combo` is
   positive and the basic-hit row of `attackId` exists (`BtlBakuganGetBasicHitParams`),
   multiplies `comboDamageScale` by `row[1] × 0.01`; then clamps `comboDamageScale` to at least
   0.1 (a NaN scale is left as is). */
void BtlBakuganDecayComboDamageScale(BtlBakugan *self, s32 attackId)
{
    if (self->combo > 0) {
        s16 *row = BtlBakuganGetBasicHitParams(self, attackId);

        if (row != NULL) {
            self->comboDamageScale =
                (float)row[1] * 0.00999999978f * self->comboDamageScale; /* 0x3c23d70a */
        }
    }
    if (self->comboDamageScale < 0.100000001f) { /* 0x3dcccccd */
        self->comboDamageScale = 0.100000001f;
    }
}
