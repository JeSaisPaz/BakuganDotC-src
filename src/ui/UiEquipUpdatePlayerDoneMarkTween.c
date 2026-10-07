// bdc 0x0895ed90 UiEquipUpdatePlayerDoneMarkTween
#include "bdc.h"

/* Advances the tween started by `UiEquipStartPlayerDoneMarkTween` for player `player` on the
   UiEquip Bakugan/gear loadout screen (task 302, `UiEquipCtor`); returns true when done. */

bool UiEquipUpdatePlayerDoneMarkTween(UiEquip *self, u8 out, u8 player)
{
  int i = self->spriteIdx[10] + player;
  GfxSprite *sprite = ((GfxSprite **)self->base.data)[i];
  if (out == 0) {
    return UiTweenUpdate(1.5f, 1.0f, 8.0f, 0, sprite, &self->tweens[i], 3) != 0;
  }
  return UiTweenUpdate(1.0f, 1.5f, 8.0f, out, sprite, &self->tweens[i], 3) != 0;
}
