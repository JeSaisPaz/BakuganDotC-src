// bdc 0x0898c8b0 UiCollectionFigureCellButtonsDone
#include "bdc.h"

/* Advances the zoom tweens (1.0↔1.5, 16 frames, 8 while paging `pageDir`) of the grid cell sprites
   of `UiCollectionFigure` (data sprites 0..5, 25..30, 7..12 and 13..18,
   each with the tween record of the same slot); `out` selects the direction. Returns true once any
   of them has finished (UiTweenUpdate returned true; they run in lockstep, so in practice all). */

bool UiCollectionFigureCellButtonsDone(UiCollectionFigure *self, u8 out)
{
  GfxSprite **sprites;
  float fromScale;
  float toScale;
  float frames;
  u8 finished;
  int i;

  fromScale = 1.0f;
  toScale = 1.5f;
  if (out == 0) {
    fromScale = 1.5f;
    toScale = 1.0f;
  }
  if (self->pageDir != 0) {
    frames = 8.0f;
  } else {
    frames = 16.0f;
  }
  finished = 0;
  for (i = 0; i < 6; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(fromScale, toScale, frames, out, sprites[i], &self->tweens[i], 3);
  }
  for (i = 25; i < 31; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(fromScale, toScale, frames, out, sprites[i], &self->tweens[i], 3);
  }
  for (i = 7; i < 13; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(fromScale, toScale, frames, out, sprites[i], &self->tweens[i], 3);
  }
  for (i = 13; i < 19; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(fromScale, toScale, frames, out, sprites[i], &self->tweens[i], 3);
  }
  return finished != 0;
}
