// bdc 0x0890e55c UiConfirmDialogUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the yes/no confirm dialog: calls the phase handler selected
   by `phase` (`+0x28`) from the 4-entry PMF table `0x08a9b500` (`UiConfirmDialogWaitPhase`,
   `UiConfirmDialogSetupPhase`, `UiConfirmDialogMainPhase`, `UiConfirmDialogExitPhase`), then
   `UiScreenUpdateCommon` (removes the task once `closeRequested` `+0x4c` is set). */

void UiConfirmDialogUpdate(CoreTask *task)

{
  UiScreen *screen = (UiScreen *)task;
  u32 phase = screen->phase;

  if ((s32)phase >= 0 && phase < 4) {
    const MemberFnPtr *e = &g_uiConfirmDialogPhaseTable[phase];
    u8 *obj = (u8 *)task + e->delta;
    void (*fn)(void *) = (void (*)(void *))e->pfn;
    if (e->index != 0) {
      const VtblEntry *v = *(const VtblEntry **)(obj + (intptr_t)e->pfn) + e->index;
      fn = (void (*)(void *))v->fn;
      obj += v->delta;
    }
    fn(obj);
  }
  UiScreenUpdateCommon(screen);
}
