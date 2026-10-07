// bdc 0x08865bdc BtlBakuganGetScaledStat44
#include "bdc.h"

/* Returns the unit's state-12 speed stat (`BtlUnitStatTable``.state12Speed`) times
   `BtlBakuganGetSpeedStatusFactor`. Used by `BtlBakuganState12Update` and
   `BtlUnitAltState12Update`. */
float BtlBakuganGetScaledStat44(BtlBakugan *bakugan)
{
    float speed = bakugan->combat.stats->state12Speed;

    return speed * BtlBakuganGetSpeedStatusFactor(bakugan);
}
