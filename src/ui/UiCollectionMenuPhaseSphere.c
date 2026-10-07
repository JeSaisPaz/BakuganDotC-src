// bdc 0x089744a4 UiCollectionMenuPhaseSphere
#include "bdc.h"

/* Phase 3 of `UiCollectionMenu`: keeps the shared background (`g_uiKeepSharedBg`
   = 1), opens the sphere collection (task 312, `UiCollectionSphereCtor`) and waits for it to end,
   then returns to the main phase at step 0x10. */

void UiCollectionMenuPhaseSphere(UiCollectionMenu *self)

{

  s32 step = (self->base).phaseStep;
  if (step < 1) {
    if (-1 < step) {
      g_uiKeepSharedBg = 1;
      CoreTaskCreateDefault(0x138,(void *)(intptr_t)self->selSub);
      (self->base).phaseStep = (self->base).phaseStep + 1;
      return;
    }
  }
  else if (step < 2) {
    if (CoreTaskExists(0x138) != 0) {
      return;
    }
    (self->base).phaseStep = 2;
    return;
  }
  (self->base).phase = 2;
  (self->base).phaseStep = 0x10;
  return;
}

