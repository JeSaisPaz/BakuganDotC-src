// bdc 0x089afe78 UiBattleModeSelectUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the UiBattleModeSelect screen (task id 350): runs the current
   phase handler from the 4-entry pointer-to-member phase table `g_uiBattleModeSelectPhaseTable` (indexed by `phase`,
   +0x28), then `UiScreenUpdateCommon` and, unless a close was requested, `UiScreenUpdateBg`. */

void UiBattleModeSelectUpdate(UiBattleModeSelect *self)

{
  u8 closing;
  s32 phase = self->base.phase;

  if (phase >= 0) {
    if (phase < 4) {
      const MemberFnPtr *member = &g_uiBattleModeSelectPhaseTable[phase];
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
