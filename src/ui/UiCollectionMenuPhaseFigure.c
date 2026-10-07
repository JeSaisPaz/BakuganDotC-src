// bdc 0x089745bc UiCollectionMenuPhaseFigure
#include "bdc.h"

/* Phase 5 of `UiCollectionMenu`: opens the figure collection (task 314,
   `UiCollectionFigureCtor`) keeping the shared background, waits for it to end and returns to the
   main phase at step 0x10. */

void UiCollectionMenuPhaseFigure(UiCollectionMenu *self)

{

  s32 step = (self->base).phaseStep;
  if (step < 1) {
    if (-1 < step) {
      g_uiKeepSharedBg = 1;
      CoreTaskCreateDefault(0x13a,(void *)(intptr_t)self->selSub);
      (self->base).phaseStep = (self->base).phaseStep + 1;
      return;
    }
  }
  else if (step < 2) {
    if (CoreTaskExists(0x13a) != 0) {
      return;
    }
    (self->base).phaseStep = 2;
    return;
  }
  (self->base).phase = 2;
  (self->base).phaseStep = 0x10;
  return;
}

