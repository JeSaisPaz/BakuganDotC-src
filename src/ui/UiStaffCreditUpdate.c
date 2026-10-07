// bdc 0x08944858 UiStaffCreditUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the staff credits screen (task id 3004): runs the phase
   handler selected by `phase` (`+0x28`) from the 4-entry PMF table `g_uiStaffCreditPhaseTable`;
   while the phase is still 2 (`UiStaffCreditMainPhase`) it runs that handler up to two more
   times (the roll goes at three steps per frame), then the common screen update. */

static void UiStaffCreditRunPhase(UiScreen *screen, s32 phase)
{
  const MemberFnPtr *member = &g_uiStaffCreditPhaseTable[phase];
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

void UiStaffCreditUpdate(UiScreen *screen)
{
  s32 phase = screen->phase;

  if (phase >= 0 && (u32)phase < 4) {
    s32 i;

    UiStaffCreditRunPhase(screen, phase);
    for (i = 0; i < 2; i++) {
      if (screen->phase != 2) {
        break;
      }
      UiStaffCreditRunPhase(screen, screen->phase);
    }
  }
  UiScreenUpdateCommon(screen);
}
