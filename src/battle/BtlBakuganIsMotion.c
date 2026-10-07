// bdc 0x08860870 BtlBakuganIsMotion
#include "bdc.h"

/* Returns whether the unit's model is playing logical motion `motion`: maps it through the unit's
   motion table (`motionTable[motion]`, read as an unsigned halfword) and tests the result with
   `GfxModelIsMotion`. */

int BtlBakuganIsMotion(BtlBakugan *bakugan, int motion)
{
    return GfxModelIsMotion(&bakugan->base, (u16)bakugan->motionTable[motion]);
}
