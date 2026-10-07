// bdc 0x089582b8 UiEquipFadeInPhase
#include "bdc.h"

/* Phase 0 of `UiEquip` (phase table `0x08a9d5f8`): step 1 starts the `"main_bg.fab"`
   background; then starts an 8-frame fade from black, plays BGM 0x17 when coming from the pause
   screen (`UiEquipCameFromPause`, `SndBgmQueuePlay`) and advances the phase. */

void UiEquipFadeInPhase(UiEquip *self)
{
  GfxFader *fader;
  s32 step = self->base.phaseStep;

  if (step <= 0) {
    if (step >= 0) {
      self->base.phaseStep = step + 1;
      return;
    }
  } else if (step < 2) {
    UiSharedAnimStart(10.0f, 0.0f, 0.0f, self, (void *)"main_bg.fab", 0, 0);
    self->base.phaseStep = self->base.phaseStep + 1;
    return;
  }
  fader = GfxGetActiveFader();
  fader->start[0] = 0.0f;
  fader->start[1] = 0.0f;
  fader->start[2] = 0.0f;
  fader->start[3] = 1.0f;
  fader = GfxGetActiveFader();
  fader->end[0] = 0.0f;
  fader->end[1] = 0.0f;
  fader->end[2] = 0.0f;
  fader->end[3] = 0.0f;
  GfxFaderStart(GfxGetActiveFader(), 8);
  if (UiEquipCameFromPause(self) != 0) {
    SndBgmQueuePlay(0, 0x17, 1, 0);
  }
  self->base.phaseStep = 0;
  self->base.phase = self->base.phase + 1;
}
