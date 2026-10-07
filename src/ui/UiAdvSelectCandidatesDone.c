// bdc 0x08919954 UiAdvSelectCandidatesDone
#include "bdc.h"

/* Advances the candidate-row tweens (sprites 5..0xa, 0xb..0x10 and 0x12..0x17; UiTweenUpdate
   1.0 -> 1.0 over 16 frames, mode 5, `hide` as fade-out) started by `UiAdvSelectTweenCandidates`
   and returns the sum of UiTweenUpdate results != 0: true once at least one of these tweens has
   finished (UiTweenUpdate returns true when its tween is done). Sprite 0x11 is skipped. */

bool UiAdvSelectCandidatesDone(UiAdvSelect *self, u8 hide)
{
  u8 running = 0;
  s32 i;

  for (i = 5; i < 0xb; i++) {
    running += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 5);
  }
  for (i = 0xb; i < 0x11; i++) {
    running += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 5);
  }
  for (i = 0x12; i < 0x18; i++) {
    running += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 5);
  }
  return running != 0;
}
