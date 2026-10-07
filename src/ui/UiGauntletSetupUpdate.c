// bdc 0x08931e8c UiGauntletSetupUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the gauntlet card setup screen (task id 373): runs the phase
   handler from the 4-entry PMF table `0x08a9c698` (`g_uiGauntletSetupPhaseTable`), then the model
   update `UiGauntletSetupUpdateModels`, `UiScreenUpdateCommon`, and unless the screen was
   closing (`closeRequested` sampled before the common update) `UiScreenUpdateBg`. */

void UiGauntletSetupUpdate(UiGauntletSetup *self)

{
  u32 phase = self->base.phase;
  u8 closing;

  if ((s32)phase >= 0 && phase < 4) {
    const MemberFnPtr *e = &g_uiGauntletSetupPhaseTable[phase];
    u8 *obj = (u8 *)self + e->delta;
    void (*fn)(void *) = (void (*)(void *))e->pfn;
    if (e->index != 0) {
      const VtblEntry *v = *(const VtblEntry **)(obj + (intptr_t)e->pfn) + e->index;
      fn = (void (*)(void *))v->fn;
      obj += v->delta;
    }
    fn(obj);
  }
  UiGauntletSetupUpdateModels(self);
  closing = self->base.closeRequested;
  UiScreenUpdateCommon(&self->base);
  if (closing == 0) {
    UiScreenUpdateBg(&self->base);
  }
}
