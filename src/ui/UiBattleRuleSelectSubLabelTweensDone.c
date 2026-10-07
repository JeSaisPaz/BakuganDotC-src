// bdc 0x08953cd8 UiBattleRuleSelectSubLabelTweensDone
#include "bdc.h"

/* Advances the sub-option label tweens of `UiBattleRuleSelect` (sprites and
   tweens 17..19, 8 frames, alpha+scale flags 3; opening scales 1.4 -> 1.0, closing 1.0 -> 1.4) and
   returns true when all three have finished. */

bool UiBattleRuleSelectSubLabelTweensDone(UiBattleRuleSelect *self, u8 closing)
{
  u8 done = 0;
  int i;

  for (i = 17; i < 20; i++) {
    GfxSprite *sprite = ((GfxSprite **)self->base.data)[i];
    if (closing == 0) {
      done += UiTweenUpdate(1.4f, 1.0f, 8.0f, 0, sprite, &self->tweens[i], 3);
    } else {
      done += UiTweenUpdate(1.0f, 1.4f, 8.0f, closing, sprite, &self->tweens[i], 3);
    }
  }
  return done == 3;
}
