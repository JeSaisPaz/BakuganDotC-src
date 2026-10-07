// bdc 0x0896ff10 UiOptionUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the UiOption screen (task id 304): runs the current phase
   handler from the 5-entry pointer-to-member phase table `g_uiOptionPhaseTable` (indexed by
   `phase`, +0x28), then `UiScreenUpdateCommon` and, unless a close was requested,
   `UiScreenUpdateBg`. */

void UiOptionUpdate(UiOption *self)

{
  u8 closing;
  s32 phase = self->base.phase;

  if (phase >= 0) {
    if (phase < 5) {
      const MemberFnPtr *member = &g_uiOptionPhaseTable[phase];
      u8 *obj = (u8 *)self + member->delta;
      void *fn = member->pfn;

      if (member->index != 0) {
        const VtblEntry *entry =
            &(*(const VtblEntry **)(obj + (uintptr_t)member->pfn))[member->index];

        fn = entry->fn;
        obj += entry->delta;
      }
      ((void (*)(void *))fn)(obj);
    }
  }
  closing = self->base.closeRequested;
  UiScreenUpdateCommon(&self->base);
  if (closing == 0) {
    UiScreenUpdateBg(&self->base);
  }
}
