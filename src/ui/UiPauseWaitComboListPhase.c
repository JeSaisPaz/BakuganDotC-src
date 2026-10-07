// bdc 0x08910ba0 UiPauseWaitComboListPhase
#include "bdc.h"

/* Phase 6 of the pause menu: waits until the combo list (task 3002) is gone, then reads script
   global word 3: 0 → phase 5 (return to the menu), 1 → phase 3 (close). */

void UiPauseWaitComboListPhase(UiPause *self)
{
  s32 v;

  if (CoreTaskExists(0xbba) == 0) {
    v = g_scriptGlobalVars[3];
    if (v < 1) {
      if (v >= 0) {
        self->base.spriteLayer->alpha = 0.0f;
        self->scaleX = 10.0f;
        self->base.phase = 5;
      }
    } else if (v < 2) {
      self->base.phase = 3;
      self->base.phaseStep = 0;
    }
  }
}
