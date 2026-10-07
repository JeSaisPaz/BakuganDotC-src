// bdc 0x0888c1ac BtlAttackParamsGetHitEnergy
#include "bdc.h"

/* Returns `BtlAttackParams``.hitEnergy` (`+0x20`, read as s16) of attack type `type`, 0 when out
   of range. `BtlBakuganOnHit` scales it by the unit stat float `stats+200` and the hit count and
   passes it as the energy amount of state 6 (getting hit) to the unit state setter. */

int BtlAttackParamsGetHitEnergy(int type)
{
    BtlAttackParams *params = BtlAttackParamsGet(type);

    if (params == NULL) {
        return 0;
    }
    return (s16)params->hitEnergy;
}
