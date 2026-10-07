// bdc 0x0896196c UiEquipUpdateDoneSlotSpriteTween
#include "bdc.h"

/* Advances the zoom tween of sprite `idx` of the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`) (`UiTweenUpdate` 1.5→1.0 in, 1.0→1.5 out, 8 frames); returns true when
   done. */

bool UiEquipUpdateDoneSlotSpriteTween(UiEquip *self, u8 out, u16 idx)
{
  GfxSprite *sprite = ((GfxSprite **)self->base.data)[idx];
  if (out == 0) {
    return UiTweenUpdate(1.5f, 1.0f, 8.0f, 0, sprite, &self->tweens[idx], 3) != 0;
  }
  return UiTweenUpdate(1.0f, 1.5f, 8.0f, out, sprite, &self->tweens[idx], 3) != 0;
}
