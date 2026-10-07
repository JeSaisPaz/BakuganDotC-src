// bdc 0x0897ac64 UiCollectionSphereUpdateFrameTween
#include "bdc.h"

/* Advances the frame/page-number tweens of `UiCollectionSphere` (sprites
   0x21, 0x23, 0x24 then 0x22; `UiTweenUpdate` scale 1 -> 1 over 16 frames, mode 1) started by
   `UiCollectionSphereStartFrameTween`; `out` selects fade-out. Returns true once any of the four
   tweens has finished (the byte sum of the UiTweenUpdate results is non-zero); they run in
   lockstep, so in practice when all are done. */

bool UiCollectionSphereUpdateFrameTween(UiCollectionSphere *self, u8 out)
{
  GfxSprite **sprites;
  u8 sum = 0;
  int i;

  for (i = 0x21; i < 0x22; i++) {
    sprites = (GfxSprite **)self->base.data;
    sum += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x23; i < 0x25; i++) {
    sprites = (GfxSprite **)self->base.data;
    sum += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x22; i < 0x23; i++) {
    sprites = (GfxSprite **)self->base.data;
    sum += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, sprites[i], &self->tweens[i], 1);
  }
  return sum != 0;
}
