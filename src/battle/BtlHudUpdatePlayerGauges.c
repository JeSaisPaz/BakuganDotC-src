// bdc 0x0882e404 BtlHudUpdatePlayerGauges
#include "bdc.h"

/* battle HUD (`BtlHudUpdate`) widget updater (first one called by `BtlHudPhaseMain`): for the
   player's Bakugan (`BtlHudGetPlayerBakugan`) updates the HP bar (`BtlHudUpdateHpBar`), the
   energy bar (`BtlHudUpdateEnergyBar`), the guard popup (`BtlHudUpdateGuardPopup`), the
   linked-object icon (`BtlHudUpdateLinkIcon`), the status icons (`BtlHudUpdateStatusIcons`),
   the end-of-battle banner (`BtlHudUpdateFinishBanner`) and the state icon
   (`BtlHudUpdateStateIcon`); nothing when there is no player Bakugan. */

void BtlHudUpdatePlayerGauges(BtlHud *self)
{
    BtlBakugan *unit;

    unit = BtlHudGetPlayerBakugan(self);
    if (unit != NULL) {
        BtlHudUpdateHpBar(self, unit);
        BtlHudUpdateEnergyBar(self, unit);
        BtlHudUpdateGuardPopup(self, unit);
        BtlHudUpdateLinkIcon(self, unit);
        BtlHudUpdateStatusIcons(self, unit);
        BtlHudUpdateFinishBanner(self);
        BtlHudUpdateStateIcon(self, unit);
    }
}
