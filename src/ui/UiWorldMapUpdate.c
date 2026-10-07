// bdc 0x089971e0 UiWorldMapUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the UiWorldMap screen (task id 310): runs the current phase
   handler from the 6-entry pointer-to-member phase table `0x08a9eeb8` (indexed by `phase`, +0x28),
   then `UiWorldMapUpdateModels`, then `UiScreenUpdateCommon` and, unless a close was requested,
   `UiScreenUpdateBg`. */

void UiWorldMapUpdate(UiScreen *screen)

{
  u8 closing;
  s32 phase = screen->phase;

  if (phase >= 0) {
    if (phase < 6) {
      const MemberFnPtr *member = &g_uiWorldMapPhaseTable[phase];
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
  UiWorldMapUpdateModels(screen);
  closing = screen->closeRequested;
  UiScreenUpdateCommon(screen);
  if (closing == 0) {
    UiScreenUpdateBg(screen);
  }
}
