// bdc 0x08862c98 BtlBakuganGetSpeedStatusFactor
#include "bdc.h"

/* Returns the speed multiplier from the unit's timed statuses: 1.4 while status 4 is active and
   x0.65 while status 5 is active, so 1.0, 1.4, 0.65 or 0.91. */
float BtlBakuganGetSpeedStatusFactor(BtlBakugan *bakugan)
{
    float factor = 1.0f;
    if (bakugan->combat.status[4].active != 0) {
        factor = 1.4f;
    }
    if (bakugan->combat.status[5].active != 0) {
        factor = factor * 0.65f;
    }
    return factor;
}
