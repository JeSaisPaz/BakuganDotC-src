// bdc 0x08865d64 BtlBakuganGetScaledStat64
#include "bdc.h"

/* Returns the unit stat `riseAccel` of its `BtlUnitStatTable` times
   `BtlBakuganGetSpeedStatusFactor`. Used by `BtlBakuganState13Update` and `BtlUnitMode4MoveTowardPoint`. */
float BtlBakuganGetScaledStat64(BtlBakugan *bakugan)
{
    float base = bakugan->combat.stats->riseAccel;

    return base * BtlBakuganGetSpeedStatusFactor(bakugan);
}
