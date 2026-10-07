// bdc 0x089893b8 UiCollectionTheaterUpdateThumbnailTween
#include "bdc.h"

/* Advances the zoom tweens (scale 1.5 ↔ 1) of the thumbnail sprites 0x0d..0x12 of
   `UiCollectionTheater` started by
   `UiCollectionTheaterStartThumbnailTween`; sums the per-sprite finished flags
   and returns true once ANY of the six tweens has finished (sum != 0). */

bool UiCollectionTheaterUpdateThumbnailTween(UiScreen *screen, u8 out)

{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  GfxSprite **sprites = (GfxSprite **)screen->data;
  int i;
  u8 finished;
  float toScale;
  float fromScale;
  float frames;

  toScale = 1.5f;
  fromScale = 1.0f;
  if (out == 0) {
    fromScale = 1.5f;
    toScale = 1.0f;
  }
  if ((s8)self->pageDir == 0) {
    frames = 16.0f;
  }
  else {
    frames = 8.0f;
  }
  finished = 0;
  for (i = 0; i < 6; i++) {
    finished = finished + UiTweenUpdate(fromScale, toScale, frames, out,
                                        sprites[13 + i],
                                        &self->thumbTweens[i], 3);
  }
  return finished != 0;
}
