// bdc 0x089399b4 UiUnlockResultExitPhase
#include "bdc.h"

/* Phase 3 of the unlock result screen: sets `closeRequested` (`+0x4c`). */

void UiUnlockResultExitPhase(UiUnlockResult *self)

{
  (self->base).closeRequested = '\x01';
  return;
}

