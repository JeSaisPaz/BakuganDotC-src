// bdc 0x08969708 UiCardEquipUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the UiCardEquip screen (task id 303): runs the current phase
   handler from the 5-entry pointer-to-member phase table `g_uiCardEquipPhaseTable` (indexed by `phase`, +0x28),
   then `UiScreenUpdateCommon` and, unless a close was requested, `UiScreenUpdateBg`. */

void UiCardEquipUpdate(UiCardEquip *self)

{
  u8 closeRequested;
  s32 phase = self->base.phase;

  if (phase >= 0 && phase < 5) {
    const MemberFnPtr *member = &g_uiCardEquipPhaseTable[phase];
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
