// bdc 0x08939918 UiUnlockResultLoadPhase
#include "bdc.h"

/* Phase 1 of the unlock result screen: loads the item sprites/model and text (`UiUnlockResultInitDigits`,
   `UiUnlockResultCreateNameText`, `UiUnlockResultCreateHelpText`, `UiUnlockResultCreateCamera`), then advances to the main phase. */

void UiUnlockResultLoadPhase(UiUnlockResult *self)

{
  if (self->base.phaseStep == 0) {
    UiUnlockResultInitDigits(self);
    UiUnlockResultCreateNameText(self);
    UiUnlockResultCreateHelpText(self);
    if (self->rewardKind == 5 || self->rewardKind == 6) {
      UiUnlockResultCreateCamera(self);
    }
    self->base.phaseStep = self->base.phaseStep + 1;
  } else {
    self->loadTimer = 8;
    self->base.phase = self->base.phase + 1;
    self->base.phaseStep = 0;
  }
  return;
}
