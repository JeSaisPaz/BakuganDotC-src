// bdc 0x0888c0c0 BtlAttackParamsGetHitEffect
#include "bdc.h"

/* Returns `BtlAttackParams``.hitEffect` (byte `+3`) of attack type `type`, 0 when out of range.
   `BtlBakuganOnHit` switches on it: 2/5 knock-down, 3 knock-down with a flag, 4 status 0x12 for
   600 frames, 6 status 0x13 for 120 frames, 7 reaction 2. */
int BtlAttackParamsGetHitEffect(int type)
{
    BtlAttackParams *params = BtlAttackParamsGet(type);

    if (params == NULL) {
        return 0;
    }
    return params->hitEffect;
}
