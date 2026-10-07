// bdc 0x088d2340 UiFieldHudUpdateGaugePanelB
#include "bdc.h"

/* Once story flag 0x38e is set, dims sprite `+0x44` of the field HUD (task 3001,
   `UiFieldHudCtor`; sprite array `+0x1c`, player `+0x74`) depending on the player's byte `+0x3a0`
   and updates gauge 1 into `blinkB` (`UiFieldHudUpdatePowerGauge`). */

void UiFieldHudUpdateGaugePanelB(UiFieldHud *self)

{
  GfxSprite *sprite;

  if (GameEventFlagTest(0x38e)) {
    sprite = ((GfxSprite **)(self->base).data)[0x11];
    if (self->player->stealth == '\0') {
      sprite->tint[2] = 1.0f;
    }
    else {
      sprite->tint[2] = 0.0f;
    }
    UiFieldHudUpdatePowerGauge(self,1,&self->blinkB);
  }
  return;
}
