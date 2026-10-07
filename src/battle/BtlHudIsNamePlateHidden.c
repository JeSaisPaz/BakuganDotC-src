// bdc 0x0883335c BtlHudIsNamePlateHidden
#include "bdc.h"

/* Returns 1 when the name plate of `unit` should be hidden: unless the flag byte `g_btlHudHidden`
   is set (then always 0, all plates visible), only the player Bakugan's current target
   (`BtlBakuganGetTarget` of `BtlGetPlayerBakugan`) gets a visible plate. */

s32 BtlHudIsNamePlateHidden(BtlHud *self, void *unit)
{
    BtlBakugan *player;
    s32 hidden;

    (void)self;
    hidden = 0;
    if (g_btlHudHidden == 0) {
        player = BtlGetPlayerBakugan();
        hidden = 1;
        if (player != NULL && BtlBakuganGetTarget(player) == unit) {
            hidden = 0;
        }
    }
    return hidden;
}
