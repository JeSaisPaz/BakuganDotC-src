// bdc 0x08995c3c UiUnlockCodeIntroPhase
#include "bdc.h"

/* Phase 2 of `UiUnlockCode`: step 0 waits for the fade-in, steps 1-2 show the
   instruction message (`UiUnlockCodeSetMessage`/`UiUnlockCodeShowMessage`: 0xe when all 8
   rewards are already registered (`registeredCount` = 8, then exits to phase 5), else 9 in 8-digit
   mode or 0xb in 10-digit mode), then steps 3-5 show and fade the frame and keyboard sprites in
   over two 6-frame steps, draw the keyboard (`UiUnlockCodeLayoutKeyboard`) and step 6+ enters
   the input phase 3. */

#define SPR(i) (((GfxSprite **)self->base.data)[i])

void UiUnlockCodeIntroPhase(UiUnlockCode *self)
{
  float t;
  float half;
  GfxSprite *spr;
  int i;

  switch ((u32)self->base.phaseStep) {
  case 0:
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      self->base.phaseStep = self->base.phaseStep + 1;
    }
    break;
  case 1:
    if (self->registeredCount == 8) {
      UiUnlockCodeSetMessage(self, 0xe);
    } else if (self->dimensionsMode == 0) {
      UiUnlockCodeSetMessage(self, 9);
    } else {
      UiUnlockCodeSetMessage(self, 0xb);
    }
    self->base.phaseStep = 2;
    break;
  case 2:
    if (UiUnlockCodeShowMessage(self) == 1) {
      if (self->registeredCount == 8) {
        self->base.phase = 5;
        self->base.phaseStep = 0;
      } else {
        self->base.phaseStep = 3;
      }
    }
    break;
  case 3:
    SPR(1)->flags |= 1;
    SPR(22)->flags |= 1;
    SPR(28)->flags |= 1;
    SPR(29)->flags |= 1;
    SPR(30)->flags |= 1;
    SPR(31)->flags |= 1;
    self->base.phaseStep = 4;
    break;
  case 4:
    self->introTimer = self->introTimer + 1;
    t = (float)self->introTimer * 0.16666667f;
    SPR(31)->alpha = t;
    SPR(30)->alpha = t;
    SPR(29)->alpha = t;
    SPR(28)->alpha = t;
    SPR(22)->alpha = t;
    SPR(1)->alpha = t;
    if (self->introTimer >= 6) {
      for (i = 2; i < 10; i++) {
        SPR(i)->flags |= 1;
      }
      SPR(12)->flags |= 1;
      SPR(13)->flags |= 1;
      SPR(14)->flags |= 1;
      SPR(15)->flags |= 1;
      SPR(18)->flags |= 1;
      if (self->dimensionsMode == 1) {
        SPR(19)->flags |= 1;
      }
      SPR(23)->flags |= 1;
      SPR(24)->flags |= 1;
      SPR(25)->flags |= 1;
      SPR(30)->flags |= 1;
      SPR(31)->flags |= 1;
      for (i = 35; i < 41; i++) {
        SPR(i)->flags |= 1;
      }
      self->introTimer = 0;
      self->base.phaseStep = 5;
    }
    break;
  case 5:
    self->introTimer = self->introTimer + 1;
    t = (float)self->introTimer * 0.16666667f;
    for (i = 2; i < 8; i++) {
      SPR(i)->alpha = t;
    }
    SPR(25)->alpha = t;
    SPR(24)->alpha = t;
    SPR(23)->alpha = t;
    SPR(19)->alpha = t;
    SPR(18)->alpha = t;
    SPR(15)->alpha = t;
    half = t * 0.5f;
    SPR(14)->alpha = t;
    spr = SPR(13);
    spr->tint[0] = half;
    spr->tint[1] = half;
    spr->tint[2] = half;
    spr->alpha = 1.0f;
    SPR(9)->alpha = half;
    SPR(8)->alpha = half;
    SPR(12)->alpha = half;
    for (i = 35; i < 41; i++) {
      SPR(i)->alpha = t;
    }
    if (self->introTimer >= 6) {
      SPR(10)->flags |= 1;
      SPR(10)->alpha = 1.0f;
      SPR(11)->flags |= 1;
      SPR(21)->alpha = 1.0f;
      SPR(20)->alpha = 1.0f;
      SPR(11)->alpha = 1.0f;
      SPR(16)->alpha = 1.0f;
      SPR(17)->alpha = 1.0f;
      SPR(26)->flags |= 1;
      /* sprite 26 tint+alpha = g_unlockCodeColorHighlight (one lv.q/sv.q quad copy) */
      spr = SPR(26);
      spr->tint[0] = g_unlockCodeColorHighlight.x;
      spr->tint[1] = g_unlockCodeColorHighlight.y;
      spr->tint[2] = g_unlockCodeColorHighlight.z;
      spr->alpha = g_unlockCodeColorHighlight.w;
      SPR(27)->alpha = 1.0f;
      UiUnlockCodeLayoutKeyboard(self);
      self->base.phaseStep = 6;
    }
    break;
  default:
    self->base.phaseStep = 0;
    self->introTimer = 0;
    self->base.phase = 3;
    break;
  }
}
