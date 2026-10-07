// bdc 0x089836e0 UiCollectionCardUpdateButtonTween
#include "bdc.h"

/* Advances the card-button tweens of the card collection screen (task 313,
   `maybe_UiScreen313Ctor`; pages of ability cards `"collection_ability_%02d"` in
   `"waku_4_a"`/`"waku_4_b"` frames, large card art `"card_L_%03d"`, help text `"DWCardHelp"`):
   sprites 0..3, 23..26, 5..8 and 9..12 run `UiTweenUpdate` (flags 3) scaling 1.0 -> 1.5 when
   `out` is set, 1.5 -> 1.0 otherwise, over 16 frames, 8 while paging (`pageDir`), 4 when paging
   fast (`fastScroll`). Returns true while any of them is still running (non-zero count of
   UiTweenUpdate results), false when all are done. */

bool UiCollectionCardUpdateButtonTween(UiCollectionCard *self, u8 out)
{
  u8 running;
  int i;
  float fromScale;
  float toScale;
  float frames;

  toScale = 1.5f;
  fromScale = 1.0f;
  if (out == 0) {
    fromScale = 1.5f;
    toScale = 1.0f;
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
  running = 0;
  for (i = 0; i < 4; i++) {
    running += UiTweenUpdate(fromScale, toScale, frames, out,
                             ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
  }
  for (i = 23; i < 27; i++) {
    running += UiTweenUpdate(fromScale, toScale, frames, out,
                             ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
  }
  for (i = 5; i < 9; i++) {
    running += UiTweenUpdate(fromScale, toScale, frames, out,
                             ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
  }
  for (i = 9; i < 13; i++) {
    running += UiTweenUpdate(fromScale, toScale, frames, out,
                             ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
  }
  return running != 0;
}
