// bdc 0x08974530 UiCollectionMenuPhaseCard
#include "bdc.h"

/* Phase 4 of `UiCollectionMenu`: opens the card collection (task 313,
   `UiCollectionCardCtor`) keeping the shared background, waits for it to end and returns to the
   main phase at step 0x10. */

void UiCollectionMenuPhaseCard(UiCollectionMenu *self)

{

  s32 step = (self->base).phaseStep;
  if (step < 1) {
    if (-1 < step) {
      g_uiKeepSharedBg = 1;
      CoreTaskCreateDefault(0x139,(void *)(intptr_t)self->selSub);
      (self->base).phaseStep = (self->base).phaseStep + 1;
      return;
    }
  }
  else if (step < 2) {
    if (CoreTaskExists(0x139) != 0) {
      return;
    }
    (self->base).phaseStep = 2;
    return;
  }
  (self->base).phase = 2;
  (self->base).phaseStep = 0x10;
  return;
}

