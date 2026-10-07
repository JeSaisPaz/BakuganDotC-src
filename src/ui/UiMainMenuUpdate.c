// bdc 0x089a45d8 UiMainMenuUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the UiMainMenu screen (task id 300): runs the current phase
   handler from the 7-entry pointer-to-member phase table `0x08a9f388` (indexed by `phase`, +0x28),
   then `UiMainMenuUpdateModels`, `UiMainMenuUpdateItemBoxLights`, then `UiScreenUpdateCommon` and, unless a close was
   requested, `UiScreenUpdateBg`. */

void UiMainMenuUpdate(UiMainMenu *self)

{
  u32 phase = self->base.phase;
  u8 closing;

  if ((s32)phase >= 0 && phase < 7) {
    const MemberFnPtr *e = &g_uiMainMenuPhaseTable[phase];
    u8 *obj = (u8 *)self + e->delta;
    void (*fn)(void *) = (void (*)(void *))e->pfn;
    if (e->index != 0) {
      const VtblEntry *v = *(const VtblEntry **)(obj + (intptr_t)e->pfn) + e->index;
      fn = (void (*)(void *))v->fn;
      obj += v->delta;
    }
    fn(obj);
  }
  UiMainMenuUpdateModels(self);
  UiMainMenuUpdateItemBoxLights(self);
  closing = self->base.closeRequested;
  UiScreenUpdateCommon(&self->base);
  if (closing == 0) {
    UiScreenUpdateBg(&self->base);
  }
}
