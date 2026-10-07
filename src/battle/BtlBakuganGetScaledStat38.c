// bdc 0x08865bb8 BtlBakuganGetScaledStat38
#include "bdc.h"

/* Returns the unit stat `state12BaseSpeed` of its `BtlUnitStatTable` times
   `BtlBakuganGetSpeedStatusFactor`. Used by `BtlBakuganState12Update` and `BtlUnitAltState12Update`. */
float BtlBakuganGetScaledStat38(BtlBakugan *bakugan)
{
    float base = bakugan->combat.stats->state12BaseSpeed;

    return base * BtlBakuganGetSpeedStatusFactor(bakugan);
}
