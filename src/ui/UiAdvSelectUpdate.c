// bdc 0x08917b58 UiAdvSelectUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the adventure character/Bakugan select screen (task id 376):
   runs the phase handler from the 4-entry pointer-to-member table `g_uiAdvSelectPhaseTable`
   (indexed by `phase`), then `UiAdvSelectUpdateModel`, then `UiScreenUpdateCommon` and, unless a
   close was requested, `UiScreenUpdateBg`. */

void UiAdvSelectUpdate(UiAdvSelect *self)

{
  u8 closeRequested;
  s32 phase = self->base.phase;

  if (phase >= 0 && phase < 4) {
    const MemberFnPtr *member = &g_uiAdvSelectPhaseTable[phase];
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
  UiAdvSelectUpdateModel(self);
  closeRequested = self->base.closeRequested;
  UiScreenUpdateCommon(&self->base);
  if (closeRequested == 0) {
    UiScreenUpdateBg(&self->base);
  }
  return;
}
