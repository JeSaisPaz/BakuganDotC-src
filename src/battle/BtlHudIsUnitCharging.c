// bdc 0x088305d4 BtlHudIsUnitCharging
#include "bdc.h"

/* True when the unit's charge flag (bit 0x400000 of its state flags) is set; the HUD's arrows
   switch to the warning style for charging enemies. The first argument is unused. */
bool BtlHudIsUnitCharging(BtlHud *self, void *unit)
{
    const BtlBakugan *bakugan = unit;

    (void)self;
    return (bakugan->stateFlags & 0x400000) != 0;
}
