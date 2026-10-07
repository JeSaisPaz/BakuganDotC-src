// bdc 0x0882f564 BtlHudGetComboCount
#include "bdc.h"

/* Returns the player Bakugan's current combo count (`combo`, see `BtlBakuganResetCombo`), or 0
   without a player Bakugan (`BtlGetPlayerBakugan`). The argument (the HUD) is unused. */

s32 BtlHudGetComboCount(BtlHud *self)
{
    BtlBakugan *player;

    (void)self;
    player = (BtlBakugan *)BtlGetPlayerBakugan();
    if (player != NULL) {
        return player->combo;
    }
    return 0;
}
