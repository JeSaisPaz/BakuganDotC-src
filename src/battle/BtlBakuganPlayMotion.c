// bdc 0x08860898 BtlBakuganPlayMotion
#include "bdc.h"

/* Starts motion `motion` on a battle unit: unless `force` is set, returns 0 when that motion is
   already playing (`BtlBakuganIsMotion`); records the motion in the unit's statistics
   (`BtlStatsSetMotion`) when it has one; maps the unit motion index to the model's motion id
   through the u16 table `motionTable` and plays it with blend time `blend` (0.2 becomes 0.4 for
   kind 10, read from the object id) and the loop flag via `GfxModelPlayMotion`, returning its
   result. */

int BtlBakuganPlayMotion(float blend, BtlBakugan *self, int motion, u8 loop, char force)
{
    if (force == 0 && BtlBakuganIsMotion(self, motion) != 0) {
        return 0;
    }
    if (self->stats != NULL) {
        BtlStatsSetMotion(self->stats, motion);
    }
    if (self->base.base.unk08 == 10 && blend == 0.200000003f) {
        blend = 0.400000006f;
    }
    return GfxModelPlayMotion(blend, &self->base, (u16)self->motionTable[motion], loop);
}
