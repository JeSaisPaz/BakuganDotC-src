// bdc 0x0897a710 UiCollectionSphereUpdateCellButtonTween
#include "bdc.h"

/* Advances the zoom tweens (scale 1.5 -> 1 when `out` is 0, 1 -> 1.5 otherwise; 16 frames, 8 while
   `pageDir` is set; flags 3) of sprites/tweens 0..5, 0x1b..0x20, 7..0xc and 0xe..0x13 (cell
   buttons, labels and markers) of `UiCollectionSphere` started by
   `UiCollectionSphereStartCellButtonTween`. Returns true when at least one `UiTweenUpdate`
   call returned non-zero (the u8 sum of the results is non-zero). */

bool UiCollectionSphereUpdateCellButtonTween(UiCollectionSphere *self, u8 out)
{
  GfxSprite **sprites;
  float fromScale;
  float toScale;
  float frames;
  u8 busy;
  int i;

  fromScale = 1.0f;
  toScale = 1.5f;
  if (out == 0) {
    fromScale = 1.5f;
    toScale = 1.0f;
  }
  if (self->pageDir == 0) {
    frames = 16.0f;
  } else {
    frames = 8.0f;
  }
  busy = 0;
  for (i = 0; i < 6; i++) {
    sprites = (GfxSprite **)self->base.data;
    busy += UiTweenUpdate(fromScale, toScale, frames, out, sprites[i], &self->tweens[i], 3);
  }
  for (i = 0x1b; i < 0x21; i++) {
    sprites = (GfxSprite **)self->base.data;
    busy += UiTweenUpdate(fromScale, toScale, frames, out, sprites[i], &self->tweens[i], 3);
  }
  for (i = 7; i < 0xd; i++) {
    sprites = (GfxSprite **)self->base.data;
    busy += UiTweenUpdate(fromScale, toScale, frames, out, sprites[i], &self->tweens[i], 3);
  }
  for (i = 0xe; i < 0x14; i++) {
    sprites = (GfxSprite **)self->base.data;
    busy += UiTweenUpdate(fromScale, toScale, frames, out, sprites[i], &self->tweens[i], 3);
  }
  return busy != 0;
}
