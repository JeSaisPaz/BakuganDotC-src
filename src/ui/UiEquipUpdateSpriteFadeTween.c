// bdc 0x08960008 UiEquipUpdateSpriteFadeTween
#include "bdc.h"

/* Advances the fade tween of sprite `idx` started by `UiEquipStartSpriteFadeTween` on the UiEquip
   Bakugan/gear loadout screen (task 302, `UiEquipCtor`) (`UiTweenUpdate` alpha 0→1 in, 1→0
   out, 8 frames); returns true when done. */

bool UiEquipUpdateSpriteFadeTween(UiEquip *self, u8 out, u16 idx)
{
  GfxSprite *sprite = ((GfxSprite **)self->base.data)[idx];
  if (out == 0) {
    return UiTweenUpdate(0.0f, 1.0f, 8.0f, 0, sprite, &self->tweens[idx], 3) != 0;
  }
  return UiTweenUpdate(1.0f, 0.0f, 8.0f, out, sprite, &self->tweens[idx], 3) != 0;
}
