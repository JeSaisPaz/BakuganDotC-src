// bdc 0x0888b7e4 UiHpGaugeIsNotPlayersTarget
#include "bdc.h"

/* Returns 0 while the flag byte `0x08aba77f` is set; otherwise 1 for an object gauge (mode 2), and
   for a unit gauge (mode 1) 1 unless its unit is the current target of the player's Bakugan
   (`BtlGetPlayerBakugan`, `BtlBakuganGetTarget`). `UiHpGaugeDraw` projects a non-player gauge
   above its unit only when this returns 0. */

s32 UiHpGaugeIsNotPlayersTarget(UiHpGauge *self)
{
    s32 result = 0;

    if (g_btlHudHidden == 0) {
        if (self->mode < 2) {
            if (self->mode > 0) {
                BtlBakugan *player = BtlGetPlayerBakugan();
                result = 1;
                if (player != NULL && BtlBakuganGetTarget(player) == self->unit) {
                    result = 0;
                }
            }
        } else if (self->mode < 3) {
            result = 1;
        }
    }
    return result;
}
