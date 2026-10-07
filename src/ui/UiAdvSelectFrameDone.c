// bdc 0x08918ad0 UiAdvSelectFrameDone
#include "bdc.h"

/* Advances the frame tweens started by `UiAdvSelectTweenFrame` (sprite 0 with tween 0 at `+0x78`,
   flags 7; sprite 2 with tween 2 at `+0xc8`, flags 0xb) with `UiTweenUpdate` over 16 frames
   (scale 1.5 → 1 when showing, 1 → 1.5 with fade-out when `hide` is set) and returns true when any
   of them reports its transition finished (the u8 sum of the results is non-zero). */

bool UiAdvSelectFrameDone(UiAdvSelect *self, u8 hide)
{
  u8 finished = 0;
  int i;

  if (hide == 0) {
    for (i = 0; i < 1; i++) {
      finished += UiTweenUpdate(1.5f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                                &self->tweens[i], 7);
    }
    for (i = 2; i < 3; i++) {
      finished += UiTweenUpdate(1.5f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                                &self->tweens[i], 0xb);
    }
  }
  else {
    for (i = 0; i < 1; i++) {
      finished += UiTweenUpdate(1.0f, 1.5f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                                &self->tweens[i], 7);
    }
    for (i = 2; i < 3; i++) {
      finished += UiTweenUpdate(1.0f, 1.5f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                                &self->tweens[i], 0xb);
    }
  }
  return finished != 0;
}
