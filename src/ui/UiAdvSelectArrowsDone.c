// bdc 0x089191b4 UiAdvSelectArrowsDone
#include "bdc.h"

/* Advances the tweens of sprites 0x18..0x1b (UiTweenUpdate 1.0 -> 1.0 over 16 frames, `hide` as fade-out,
   flags 1) and returns true once any of them has finished (they run in lockstep, so: arrows done). */

bool UiAdvSelectArrowsDone(UiAdvSelect *self, u8 hide)
{
  u8 finished = 0;
  s32 i;

  for (i = 0x18; i < 0x1c; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 1);
  }
  return finished != 0;
}
