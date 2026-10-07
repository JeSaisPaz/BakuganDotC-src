// bdc 0x0888c118 BtlAttackParamsGetKnockdownPower
#include "bdc.h"

/* Returns `BtlAttackParams``.knockdownPower` (`+0x18`, read as s16) of attack type `type`, 0 when
   out of range. Multiplied by the hit count and added to the target's knock-down gauge
   (`unit+0x1d4`, ≥ 100 knocks down) by `BtlBakuganOnHit` and `BtlBakuganApplyQueuedHits`. */
int BtlAttackParamsGetKnockdownPower(int type)
{
    BtlAttackParams *params = BtlAttackParamsGet(type);

    if (params == NULL) {
        return 0;
    }
    return (s16)params->knockdownPower;
}
