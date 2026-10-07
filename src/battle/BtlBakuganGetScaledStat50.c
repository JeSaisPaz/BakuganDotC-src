// bdc 0x08865d10 BtlBakuganGetScaledStat50
#include "bdc.h"

/* Returns the unit stat `dashSpeed` of its `BtlUnitStatTable` times
   `BtlBakuganGetSpeedStatusFactor`. Used by `BtlBakuganDashStep` and `BtlUnitMode4MoveTowardPoint` (dash speed). */
float BtlBakuganGetScaledStat50(BtlBakugan *bakugan)
{
    float base = bakugan->combat.stats->dashSpeed;

    return base * BtlBakuganGetSpeedStatusFactor(bakugan);
}
