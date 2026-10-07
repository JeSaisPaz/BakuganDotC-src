// bdc 0x08953abc UiBattleRuleSelectSubOptionTweensDone
#include "bdc.h"

/* Advances the three sub-option button tweens of `UiBattleRuleSelect`
   (tween/sprite slots 11–13, or 14–16 when `SaveGetProfileFlag0` is set) by one frame of
   `UiTweenUpdate` (8 frames, flags 3; scale 1.4→1 opening, 1→1.4 when `closing`) and returns
   true when all three report finished. */

bool UiBattleRuleSelectSubOptionTweensDone(UiBattleRuleSelect *self, u8 closing)
{
  u32 first;
  u32 last;
  u32 i;
  u8 doneCount;
  GfxSprite *sprite;

  if (SaveGetProfileFlag0() != 0) {
    first = 0xe;
    last = 0x10;
  }
  else {
    first = 0xb;
    last = 0xd;
  }
  doneCount = 0;
  for (i = first; i <= last; i++) {
    sprite = ((GfxSprite **)self->base.data)[i];
    if (closing == 0) {
      doneCount += UiTweenUpdate(1.4f, 1.0f, 8.0f, closing, sprite, &self->tweens[i], 3);
    }
    else {
      doneCount += UiTweenUpdate(1.0f, 1.4f, 8.0f, closing, sprite, &self->tweens[i], 3);
    }
  }
  return doneCount == 3;
}
