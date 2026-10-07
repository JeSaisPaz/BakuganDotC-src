// bdc 0x08984330 UiCollectionCardUpdateCardArtTween
#include "bdc.h"

/* Advances the card art zoom tweens (sprites/tweens 13..16) of
   `UiCollectionCard` (scale 0.3 -> 0.45 when `out`, 0.45 -> 0.3 otherwise;
   16 frames, 8 during page changes, 4 in fast scrolling); returns true once any `UiTweenUpdate`
   reports its transition finished (the four run in lockstep, so: when the zoom is done). */

bool UiCollectionCardUpdateCardArtTween(UiCollectionCard *self, u8 out)
{
  float fromScale;
  float toScale;
  float frames;
  u8 busy;
  int i;

  fromScale = 0.3f;
  toScale = 0.45000002f;
  if (out == 0) {
    fromScale = 0.45000002f;
    toScale = 0.3f;
  }
  if (self->pageDir == 0) {
    frames = 16.0f;
  }
  else if (self->fastScroll == 0) {
    frames = 8.0f;
  }
  else {
    frames = 4.0f;
  }
  busy = 0;
  for (i = 13; i < 17; i++) {
    busy += UiTweenUpdate(fromScale, toScale, frames, out, ((GfxSprite **)self->base.data)[i],
                          &self->tweens[i], 3);
  }
  return busy != 0;
}
