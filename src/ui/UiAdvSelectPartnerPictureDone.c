// bdc 0x08918ed8 UiAdvSelectPartnerPictureDone
#include "bdc.h"

/* Advances the tweens of sprites 3 and 0x25 (UiTweenUpdate 1.0 -> 1.0 over 16 frames, mode 1, `hide`
   as fade-out) started by `UiAdvSelectTweenPartnerPicture` and returns true once any of them has
   finished (UiTweenUpdate returns true when its tween is done). */

bool UiAdvSelectPartnerPictureDone(UiAdvSelect *self, u8 hide)
{
  u8 running = 0;
  s32 i;

  for (i = 3; i < 4; i++) {
    running += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 1);
  }
  for (i = 0x25; i < 0x26; i++) {
    running += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                             &self->tweens[i], 1);
  }
  return running != 0;
}
