// bdc 0x0882e2c0 BtlHudUpdateFinishBanner
#include "bdc.h"

/* Dispatches the end-of-battle banner by mode `finishMode` (`+0xa14`): 0 →
   `BtlHudAnimateFinishBanner` on HUD sprite 0x84 (byte offset `0x210` of the sprite array, read
   for every mode), 1/2 → `BtlHudAnimateFinishBannerPair`, any other mode does nothing. */
void BtlHudUpdateFinishBanner(BtlHud *self)
{
    GfxSprite *sprite = self->sprites[0x84];
    s32 mode = self->finishMode;

    if (mode == 0) {
        BtlHudAnimateFinishBanner(self, sprite);
    } else if (mode == 1 || mode == 2) {
        BtlHudAnimateFinishBannerPair(self);
    }
}
