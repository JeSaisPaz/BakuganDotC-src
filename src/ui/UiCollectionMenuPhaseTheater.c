// bdc 0x08974648 UiCollectionMenuPhaseTheater
#include "bdc.h"

/* Phase 6 of `UiCollectionMenu`: opens the theater (task 315,
   `UiCollectionTheaterCtor`, priority 100) keeping the shared background, waits for it to end and
   returns to the main phase at step 0x10. */

void UiCollectionMenuPhaseTheater(UiCollectionMenu *self)

{
  s32 step = (self->base).phaseStep;
  if (step < 1) {
    if (-1 < step) {
      g_uiKeepSharedBg = 1;
      CoreTaskCreate(0x13b,100);
      (self->base).phaseStep = (self->base).phaseStep + 1;
      return;
    }
  }
  else if (step < 2) {
    if (CoreTaskExists(0x13b) != 0) {
      return;
    }
    (self->base).phaseStep = 2;
    return;
  }
  (self->base).phase = 2;
  (self->base).phaseStep = 0x10;
  return;
}
