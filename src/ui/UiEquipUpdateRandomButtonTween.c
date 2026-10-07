// bdc 0x0895d558 UiEquipUpdateRandomButtonTween
#include "bdc.h"

/* Advances the tween started by `UiEquipStartRandomButtonTween` of the random-pick button below
   the grid (sprite range `+0x5168`, one sprite) on the UiEquip Bakugan/gear loadout screen (task
   302, `UiEquipCtor`) (`UiTweenUpdate`, 1.5→1.0 in, 1.0→1.5 out, 8 frames); returns true
   when any tween reports done (the u8 sum of the tween results is non-zero). */

bool UiEquipUpdateRandomButtonTween(UiEquip *self, u8 out)
{
  int i;
  u8 done = 0;

  for (i = self->spriteIdx[4]; i < self->spriteIdx[4] + 1; i++) {
    GfxSprite *sprite = ((GfxSprite **)self->base.data)[i];
    if (out == 0) {
      done += UiTweenUpdate(1.5f, 1.0f, 8.0f, out, sprite, &self->tweens[i], 3);
    } else {
      done += UiTweenUpdate(1.0f, 1.5f, 8.0f, out, sprite, &self->tweens[i], 3);
    }
  }
  return done != 0;
}
