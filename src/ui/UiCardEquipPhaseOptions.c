// bdc 0x0896b4d8 UiCardEquipPhaseOptions
#include "bdc.h"

/* Phase 3 of `UiCardEquip`: opens the options screen (task 304, `UiOptionCtor`)
   keeping the shared background, waits for it to close, refreshes the cursor slot
   (`UiCardEquipSetRowFocus(screen, +0x77, 1)`) and returns to phase 2 step 2. */

void UiCardEquipPhaseOptions(UiCardEquip *self)

{
  
  int step = (self->base).phaseStep;
  if (step < 1) {
    if (-1 < step) {
      g_uiKeepSharedBg = 1;
      CoreTaskCreate(0x130,100);
      (self->base).phaseStep = (self->base).phaseStep + 1;
      return;
    }
  }
  else if (step < 2) {
    s32 exists = CoreTaskExists(0x130);
    if (exists != 0) {
      return;
    }
    (self->base).phaseStep = 2;
    return;
  }
  UiCardEquipSetRowFocus(self,self->row,'\x01');
  (self->base).phase = 2;
  (self->base).phaseStep = 2;
  return;
}

