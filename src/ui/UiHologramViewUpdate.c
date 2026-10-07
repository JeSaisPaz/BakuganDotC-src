// bdc 0x08929268 UiHologramViewUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the hologram detail view screen (task id 392): runs the phase
   handler from the 4-entry table `g_uiHologramViewPhaseTable` and `UiScreenUpdateCommon`. */

void UiHologramViewUpdate(UiHologramView *self)

{
  u8 closeRequested;
  s32 phase = self->base.phase;

  if (phase >= 0 && phase < 4) {
    const MemberFnPtr *member = &g_uiHologramViewPhaseTable[phase];
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
  closeRequested = self->base.closeRequested;
  UiScreenUpdateCommon(&self->base);
  if (closeRequested == 0) {
    UiScreenUpdateBg(&self->base);
  }
  return;
}
