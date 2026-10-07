// bdc 0x0895cb90 UiEquipUpdateEmblemZoomIn
#include "bdc.h"

/* Advances the emblem zoom-in started by `UiEquipStartEmblemZoomIn` on the UiEquip Bakugan/gear
   loadout screen (task 302, `UiEquipCtor`) (`UiTweenUpdate` 4.0→3.0 over 8 frames) and caps
   the emblem alpha at 0.6; returns true when done. */

bool UiEquipUpdateEmblemZoomIn(UiEquip *self)
{
  bool done = UiTweenUpdate(4.0f, 3.0f, 8.0f, 0,
                            ((GfxSprite **)self->base.data)[self->spriteIdx[11]],
                            &self->tweens[self->spriteIdx[11]], 3) != 0;
  GfxSprite *sprite = ((GfxSprite **)self->base.data)[self->spriteIdx[11]];
  if (!(sprite->alpha <= 0.6f)) {
    sprite->alpha = 0.6f;
  }
  return done;
}
