// bdc 0x08865dac BtlBakuganGetScaledStat6C
#include "bdc.h"

/* Returns the unit stat `maxRiseSpeed` of the Bakugan's `BtlUnitStatTable` ×
   `BtlBakuganGetSpeedStatusFactor`. Used by `BtlBakuganState13Update` and `BtlUnitMode4MoveTowardPoint`. */
float BtlBakuganGetScaledStat6C(BtlBakugan *bakugan)
{
    float value = bakugan->combat.stats->maxRiseSpeed;

    return value * BtlBakuganGetSpeedStatusFactor(bakugan);
}
