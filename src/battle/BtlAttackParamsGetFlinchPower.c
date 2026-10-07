// bdc 0x0888c14c BtlAttackParamsGetFlinchPower
#include "bdc.h"

/* Returns `BtlAttackParams``.flinchPower` (`+0x1c`, read as s16) of attack type `type`, 0 when
   out of range. Multiplied by the hit count and added to the target's flinch gauge (`unit+0x1cc`,
   ≥ 100 full stagger) by `BtlBakuganOnHit` and `BtlBakuganApplyQueuedHits`. */

int BtlAttackParamsGetFlinchPower(int type)
{
    BtlAttackParams *params = BtlAttackParamsGet(type);

    if (params == NULL) {
        return 0;
    }
    return (s16)params->flinchPower;
}
