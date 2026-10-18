// bdc 0x08983c50 UiCollectionCardUpdateFrameTween
#include "bdc.h"

/* Advances the frame/page-number tweens of `UiCollectionCard` (sprites
   and tweens 0x1b, 0x1d..0x1e, then 0x1c; 16 frames, scale 1.0, flags 1) started by
   `UiCollectionCardStartFrameTween`; `out` is the fade direction. Returns true once any of them
   has finished (`UiTweenUpdate` returns true when done; the u8 sum of the results is non-zero). */

bool UiCollectionCardUpdateFrameTween(UiCollectionCard *self, u8 out)
{
  GfxSprite **sprites;
  u8 finished;
  int i;

  finished = 0;
  for (i = 0x1b; i < 0x1c; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x1d; i < 0x1f; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, sprites[i], &self->tweens[i], 1);
  }
  for (i = 0x1c; i < 0x1d; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, out, sprites[i], &self->tweens[i], 1);
  }
  return finished != 0;
}
