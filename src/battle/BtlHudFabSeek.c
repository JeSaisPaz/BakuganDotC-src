// bdc 0x0882ffbc BtlHudFabSeek
#include "bdc.h"

/* Seeks `.fab` animation `index` of the HUD's animation set `fabs` (`+0xa8c`) to frame `frame`
   (`GfxFabSeek`), when that slot is non-NULL. */
void BtlHudFabSeek(BtlHud *self, u32 frame, s32 index)
{
    if (self->fabs[index] != NULL) {
        GfxFabSeek(self->fabs[index], frame);
    }
}
