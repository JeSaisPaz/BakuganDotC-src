// bdc 0x0893ae08 UiUnlockResultNameTextFadeDone
#include "bdc.h"

/* Advances the name-text fade of `UiUnlockResult` by 1/8 (overshoot in, t²
   out) into alpha `+0x740`, marks it dirty and returns 1 when finished; returns 1 at once for kinds
   without a name (0, 3, 4, 5, 7 and out of range). */

u8 UiUnlockResultNameTextFadeDone(UiUnlockResult *self, u8 closing)
{
  float t;
  float base;
  u8 done = 0;

  switch (self->rewardKind) {
  case 1:
  case 2:
  case 6:
  case 8:
  case 9:
    t = self->nameFadeT + 0.125f;
    base = self->textFadeBase[0];
    break;
  default:
    return 1;
  }
  if (closing == 0) {
    self->nameFadeT = t;
    self->textAlpha[0] = base + (1.0f - (t - 1.0f) * (t - 1.0f));
    if (!(t < 1.0f)) {
      self->textAlpha[0] = 1.0f;
      done = 1;
    }
  } else {
    self->nameFadeT = t;
    self->textAlpha[0] = base - t * t;
    if (!(t < 1.0f)) {
      done = 1;
      self->textAlpha[0] = 0.0f;
    }
  }
  self->textVisible[0] = 1;
  return done;
}
