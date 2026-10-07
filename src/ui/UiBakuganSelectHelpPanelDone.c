// bdc 0x0892d884 UiBakuganSelectHelpPanelDone
#include "bdc.h"

/* Advances one frame of the help-panel appear/disappear transition of the Bakugan select screen
   (`UiBakuganSelectCtor`): runs `UiTweenUpdate` (scale 1 -> 1 over 16 frames, flags 1,
   `fadeOut = hide`) on sprites 0x5a..0x5d and then sprite 1, each with its own tween. Returns true
   when at least one of these tweens reports finished (they run in lockstep, so in practice when
   the whole transition is done). */

bool UiBakuganSelectHelpPanelDone(UiBakuganSelect *self, u8 hide)
{
  u8 finished;
  int i;

  finished = 0;
  for (i = 0x5a; i < 0x5e; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  }
  for (i = 1; i < 2; i++) {
    finished += UiTweenUpdate(1.0f, 1.0f, 16.0f, hide, ((GfxSprite **)self->base.data)[i],
                              &self->tweens[i], 1);
  }
  return finished != 0;
}
