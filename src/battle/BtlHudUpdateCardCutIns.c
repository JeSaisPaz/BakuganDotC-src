// bdc 0x088331dc BtlHudUpdateCardCutIns
#include "bdc.h"

/* HUD widget: when there is a player Bakugan (`BtlHudGetPlayerBakugan`), updates the charge
   gauges for it (`BtlHudUpdateChargeGauges`), then the ability-card cut-in for
   `abilityCutInUnit` (`BtlHudUpdateAbilityCutIn`) and the gate-card cut-in for `gateCutInUnit`
   (`BtlHudUpdateGateCardCutIn`); does nothing without a player Bakugan. */
void BtlHudUpdateCardCutIns(BtlHud *self)
{
    void *player = BtlHudGetPlayerBakugan(self);

    if (player != NULL) {
        BtlHudUpdateChargeGauges(self, player);
        BtlHudUpdateAbilityCutIn(self, self->abilityCutInUnit);
        BtlHudUpdateGateCardCutIn(self, self->gateCutInUnit);
    }
}
