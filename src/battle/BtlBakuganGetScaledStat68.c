// bdc 0x08865d88 BtlBakuganGetScaledStat68
#include "bdc.h"

/* Returns the unit stat `fallAccel` of the Bakugan's `BtlUnitStatTable` ×
   `BtlBakuganGetSpeedStatusFactor`. Used by `BtlBakuganState13Update`. */
float BtlBakuganGetScaledStat68(BtlBakugan *bakugan)
{
    float value = bakugan->combat.stats->fallAccel;

    return value * BtlBakuganGetSpeedStatusFactor(bakugan);
}
