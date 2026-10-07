// bdc 0x0893814c UiUnlockResultUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the unlock result screen (task id 375): runs the phase
   handler from the 4-entry table `g_uiUnlockResultPhaseTable` and `UiScreenUpdateCommon`;
   `UiScreenUpdateBg` unless the close request was already set. */

void UiUnlockResultUpdate(UiUnlockResult *self)

{
  u8 closeRequested;
  s32 phase = self->base.phase;

  if (phase >= 0 && phase < 4) {
    const MemberFnPtr *member = &g_uiUnlockResultPhaseTable[phase];
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
