// bdc 0x0890f774 UiRepairUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the card repair screen (task id 430): runs the phase handler
   from the member-pointer table `0x08a9b560` (`g_uiRepairPhaseTable`, 4 entries) and then
   `UiScreenUpdateCommon`. */

void UiRepairUpdate(UiScreen *screen)

{
  s32 phase = screen->phase;

  if (phase >= 0) {
    if (phase < 4) {
      const MemberFnPtr *member = &g_uiRepairPhaseTable[phase];
      u8 *obj = (u8 *)screen + member->delta;
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
  UiScreenUpdateCommon(screen);
}
