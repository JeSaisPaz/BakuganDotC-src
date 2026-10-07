// bdc 0x0895d344 UiEquipUpdateGridPanelTween
#include "bdc.h"

/* Advances the tween started by `UiEquipStartGridPanelTween` of the grid panel sprite (sprite
   range `spriteIdx[0]`, one sprite) on the UiEquip Bakugan/gear loadout screen (task 302,
   `UiEquipCtor`) with `UiTweenUpdate` (scale 1.5→1.0 in, 1.0→1.5 out, 8 frames, mode 3);
   returns true once the tween has finished (any UiTweenUpdate in the range returned true), false
   while it is still running. */

bool UiEquipUpdateGridPanelTween(UiEquip *self, u8 out)

{
  GfxSprite *sprite;
  u8 finished;
  u32 i;
  u32 start = self->spriteIdx[0];

  /* the asm also reads playerCount (< 3) but both branches use the same one-sprite range */
  finished = 0;
  for (i = start; i <= start; i++) {
    sprite = ((GfxSprite **)self->base.data)[i];
    if (out == 0) {
      finished += UiTweenUpdate(1.5f, 1.0f, 8.0f, out, sprite, &self->tweens[i], 3);
    }
    else {
      finished += UiTweenUpdate(1.0f, 1.5f, 8.0f, out, sprite, &self->tweens[i], 3);
    }
  }
  return finished != 0;
}
