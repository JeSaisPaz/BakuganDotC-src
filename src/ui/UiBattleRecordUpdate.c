// bdc 0x08948808 UiBattleRecordUpdate
#include "bdc.h"

/* Update method (vtable `0x08af4d44` slot 2) of the battle-record screen
   (`UiBattleRecord`): runs the current phase `+0x28` (0..5) through the
   member-pointer table `0x08a9d19c`, then `UiScreenUpdateCommon` and, unless `closeRequested`
   (`+0x4c`) is set, `UiScreenUpdateBg`. */

void UiBattleRecordUpdate(UiBattleRecord *self)

{
  u8 closing;
  s32 phase = self->base.phase;

  if (phase >= 0) {
    if (phase < 6) {
      const MemberFnPtr *member = &g_uiBattleRecordPhaseTable[phase];
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
