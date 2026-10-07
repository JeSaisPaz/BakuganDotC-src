// bdc 0x0888bff4 BtlAttackParamsCopy
#include "bdc.h"

/* Copies the 0x44-byte `BtlAttackParams` of attack type `type` (type 0's entry when
   `BtlAttackParamsGet` returns NULL for `type`) into `dst` after zeroing it. Used by
   `BtlAttackInit` and `BtlCombatCalcAbilityDamage`. The binary copies the entry as 17 words. */
void BtlAttackParamsCopy(BtlAttackParams *dst, int type)
{
    const BtlAttackParams *src = BtlAttackParamsGet(type);

    if (src == NULL) {
        src = BtlAttackParamsGet(0);
    }
    memset(dst, 0, sizeof(*dst));
    *dst = *src;
}
