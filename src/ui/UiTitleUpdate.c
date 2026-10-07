// bdc 0x08950ca8 UiTitleUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the UiTitle screen (task id 200): runs the current phase
   handler from the 7-entry pointer-to-member phase table `g_uiTitlePhaseTable` (indexed by
   `phase`, +0x28), then `UiScreenUpdateCommon`. */

void UiTitleUpdate(UiScreen *screen)
{
  s32 phase = screen->phase;

  if (phase >= 0) {
    if (phase < 7) {
      const MemberFnPtr *member = &g_uiTitlePhaseTable[phase];
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
