// bdc 0x0888a0c8 UiHpGaugeIsLocalPlayer
#include "bdc.h"

/* Returns true when the HUD hit-point gauge (`UiHpGaugeInit`, 0xa0 bytes) is bound to a unit
   (mode 1, `+0x8c`) that is the local player (`BtlBakuganIsLocalPlayer` on `+0x28`). The player's
   gauge is drawn in the fixed HUD position and with a wider bar (`UiHpGaugeGetBarWidth`,
   `UiHpGaugeEmitBars`, `UiHpGaugeDraw`). */

s32 UiHpGaugeIsLocalPlayer(UiHpGauge *self)
{
  if (self->mode < 2 && 0 < self->mode) {
    return BtlBakuganIsLocalPlayer(self->unit);
  }
  return 0;
}
