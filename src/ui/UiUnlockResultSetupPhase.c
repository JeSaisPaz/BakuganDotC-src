// bdc 0x08939494 UiUnlockResultSetupPhase
#include "bdc.h"

/* Phase 0 of the unlock result screen: builds the screen (`UiUnlockResultSetupReward`), waits a frame, then
   advances. */

void UiUnlockResultSetupPhase(UiUnlockResult *self)

{
  if ((self->base).phaseStep == 0) {
    UiUnlockResultSetupReward(self);
    (self->base).phaseStep = (self->base).phaseStep + 1;
  }
  else {
    (self->base).phaseStep = 0;
    (self->base).phase = (self->base).phase + 1;
  }
  return;
}

