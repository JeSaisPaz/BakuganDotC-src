// bdc 0x0888c180 BtlAttackParamsGetHitCount
#include "bdc.h"

/* Returns `BtlAttackParams``.hitCount` (byte `+0xf`) of attack type `type`, 0 when out of range:
   the number of hits an attack counts for (multiplies the gauge powers and is added to the
   attacker's combo). */
int BtlAttackParamsGetHitCount(int type)
{
    BtlAttackParams *params = BtlAttackParamsGet(type);

    if (params == NULL) {
        return 0;
    }
    return params->hitCount;
}
