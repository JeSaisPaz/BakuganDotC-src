// bdc 0x0895262c UiBattleRuleSelectUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the UiBattleRuleSelect screen (task id 340): runs the current
   phase handler from the 5-entry pointer-to-member phase table `g_uiBattleRuleSelectPhaseTable` (indexed by `phase`,
   +0x28), then `NetPlayHasManager`, `NetPlayGetManager`, `NetPlaySetFlags`,
   `NetPlayClearFlags`, then `UiScreenUpdateCommon` and, unless a close was requested,
   `UiScreenUpdateBg`. */

void UiBattleRuleSelectUpdate(UiBattleRuleSelect *self)

{
  u8 closing;
  s32 phase = self->base.phase;

  if (phase >= 0) {
    if (phase < 5) {
      const MemberFnPtr *member = &g_uiBattleRuleSelectPhaseTable[phase];
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
  } else if (NetPlayHasManager()) {
    NetPlayClearFlags((NetPlay *)NetPlayGetManager(), 0x8000000);
    NetPlaySetFlags((NetPlay *)NetPlayGetManager(), 0x1000000);
  }
}
