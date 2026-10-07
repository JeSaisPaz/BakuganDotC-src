// bdc 0x089b301c UiComboListUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the combo list screen (task id 3002): runs the phase handler
   from the 4-entry table `g_uiComboListPhaseTable`; once the phase passes the table it removes
   the task (`CoreTaskRemove`). */

void UiComboListUpdate(UiComboList *self)

{
  s32 phase = self->base.phase;

  if (phase >= 0) {
    if (phase < 4) {
      const MemberFnPtr *member = &g_uiComboListPhaseTable[phase];
      u8 *obj = (u8 *)self + member->delta;
      void *fn = member->pfn;

      if (member->index != 0) {
        const VtblEntry *entry =
            &(*(const VtblEntry **)(obj + (uintptr_t)member->pfn))[member->index];

        fn = entry->fn;
        obj += entry->delta;
      }
      ((void (*)(void *))fn)(obj);
      return;
    }
    CoreTaskRemove((CoreTask *)self, true);
  }
  return;
}
