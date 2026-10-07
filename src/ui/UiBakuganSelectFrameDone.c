// bdc 0x0892d3c4 UiBakuganSelectFrameDone
#include "bdc.h"

/* Advances the frame tweens of `UiBakuganSelectTweenFrame` by one frame: sprites/tweens 16..20
   with the pulse helper `UiSpriteEaseStep` (flags 3), 21 with `UiTweenUpdate` flags 0xb and
   22..23 with flags 7, scaling 1.5 -> 1.0 when showing (`hide == 0`) or 1.0 -> 1.5 when hiding,
   over 16 frames. The callees return true once their tween has finished; the 8-bit sum counts
   finished tweens, and the function returns true once any of them has finished (count != 0). */

bool UiBakuganSelectFrameDone(UiBakuganSelect *self, u8 hide)
{
  u8 finished;
  int i;

  finished = 0;
  if (hide == 0) {
    for (i = 16; i < 21; i++) {
      finished += UiSpriteEaseStep(1.5f, 1.0f, 16.0f, 0.3f, hide,
                                  ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 21; i < 22; i++) {
      finished += UiTweenUpdate(1.5f, 1.0f, 16.0f, hide,
                               ((GfxSprite **)self->base.data)[i], &self->tweens[i], 0xb);
    }
    for (i = 22; i < 24; i++) {
      finished += UiTweenUpdate(1.5f, 1.0f, 16.0f, hide,
                               ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
    }
  }
  else {
    for (i = 16; i < 21; i++) {
      finished += UiSpriteEaseStep(1.0f, 1.5f, 16.0f, 0.3f, hide,
                                  ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 21; i < 22; i++) {
      finished += UiTweenUpdate(1.0f, 1.5f, 16.0f, hide,
                               ((GfxSprite **)self->base.data)[i], &self->tweens[i], 0xb);
    }
    for (i = 22; i < 24; i++) {
      finished += UiTweenUpdate(1.0f, 1.5f, 16.0f, hide,
                               ((GfxSprite **)self->base.data)[i], &self->tweens[i], 7);
    }
  }
  return finished != 0;
}
