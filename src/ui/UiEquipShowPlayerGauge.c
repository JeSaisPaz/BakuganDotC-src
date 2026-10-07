// bdc 0x0895b6ac UiEquipShowPlayerGauge
#include "bdc.h"

/* Shows or hides the 7-part gauge of player `player` (sprites from UiEquipGetPlayerSpriteBase)
   on the UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`). Showing tints it in the
   attribute colour of `bakuganId` (`UiBakuganTintTextByAttribute`), makes the parts visible at alpha 1 on layer 4,
   sets their Y to ±0x88 px in the four-player layout (-0x88 for players 0/1, else 0x88; 0 with two players)
   and resets both gauge scroll values `gaugeScroll[player]` to 0; hiding clears the visible bit and sets
   both values to -1 (the "hidden" marker read by UiEquipUpdatePlayerGauge). */

void UiEquipShowPlayerGauge(UiEquip *self, bool hide, u8 player, u8 bakuganId)
{
  u32 first;
  u32 end;
  u32 i;
  s32 offsetY;

  first = (u32)UiEquipGetPlayerSpriteBase(self, player);
  offsetY = 0;
  end = first + 7;
  if (self->playerCount >= 3) {
    offsetY = 0x88;
    if (player < 2) {
      offsetY = -0x88;
    }
  }
  if (!hide) {
    UiBakuganTintTextByAttribute(((GfxSprite **)self->base.data)[first], bakuganId);
    for (i = first; i < end; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->alpha = 1.0f;
      ((GfxSprite **)self->base.data)[i]->layerMask = 4;
      ((GfxSprite **)self->base.data)[i]->posY = (float)offsetY;
    }
    self->gaugeScroll[player].scrollFast = 0.0f;
    self->gaugeScroll[player].scrollSlow = 0.0f;
  }
  else {
    for (i = first; i < end; i++) {
      ((GfxSprite **)self->base.data)[i]->flags &= ~1u;
    }
    self->gaugeScroll[player].scrollFast = -1.0f;
    self->gaugeScroll[player].scrollSlow = -1.0f;
  }
}
