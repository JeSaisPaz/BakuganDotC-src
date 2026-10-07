// bdc 0x08865d40 BtlBakuganGetScaledStat60
#include "bdc.h"

/* Returns the unit stat `dashTargetSpeed` of the Bakugan's `BtlUnitStatTable` ×
   `BtlBakuganGetSpeedStatusFactor`. Used by `BtlBakuganState13Update`. */
float BtlBakuganGetScaledStat60(BtlBakugan *bakugan)
{
    float value = bakugan->combat.stats->dashTargetSpeed;

    return value * BtlBakuganGetSpeedStatusFactor(bakugan);
}
