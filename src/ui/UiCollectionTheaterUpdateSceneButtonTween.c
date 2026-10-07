// bdc 0x089886d8 UiCollectionTheaterUpdateSceneButtonTween
#include "bdc.h"

/* Advances the zoom tweens (scale 1.5 ↔ 1; 16 frames, 8 during page changes) of the scene buttons
   and labels of `UiCollectionTheater` (data sprites 0..5, 31..36, 7..12
   and 19..24, each with the tween record of the same slot) started by
   `UiCollectionTheaterStartSceneButtonTween`; returns true once any of them has finished
   (UiTweenUpdate returned true; they run in lockstep, so in practice all). */

bool UiCollectionTheaterUpdateSceneButtonTween(UiScreen *screen, u8 out)
{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
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
  if ((s8)self->pageDir != 0) {
    frames = 8.0f;
  } else {
    frames = 16.0f;
  }
  finished = 0;
  for (i = 0; i < 6; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(fromScale, toScale, frames, out, sprites[i],
                             &self->sceneButtonTweens[i], 3);
  }
  for (i = 31; i < 37; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(fromScale, toScale, frames, out, sprites[i],
                             &self->sceneTweensD[i - 31], 3);
  }
  for (i = 7; i < 13; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(fromScale, toScale, frames, out, sprites[i],
                             &self->sceneTweensB[i - 7], 3);
  }
  for (i = 19; i < 25; i++) {
    sprites = (GfxSprite **)self->base.data;
    finished += UiTweenUpdate(fromScale, toScale, frames, out, sprites[i],
                             &self->sceneTweensC[i - 19], 3);
  }
  return finished != 0;
}
