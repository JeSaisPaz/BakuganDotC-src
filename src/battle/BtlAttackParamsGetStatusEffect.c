// bdc 0x0888c0ec BtlAttackParamsGetStatusEffect
#include "bdc.h"

/* Returns `BtlAttackParams``.statusEffect` (byte `+9`) of attack type `type`, 0 when out of
   range. `BtlBakuganOnHit` maps 1/2/3 to `BtlCombatApplyStatus` statuses 1/3/5 for 300 frames,
   with effects 0x53/0x55/0x54. */
int BtlAttackParamsGetStatusEffect(int type)
{
    BtlAttackParams *params = BtlAttackParamsGet(type);

    if (params == NULL) {
        return 0;
    }
    return params->statusEffect;
}
