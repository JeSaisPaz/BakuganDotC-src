// bdc 0x0890e55c UiConfirmDialogUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the yes/no confirm dialog: calls the phase handler selected
   by `phase` (`+0x28`) from the 4-entry PMF table `0x08a9b500` (`UiConfirmDialogWaitPhase`,
   `UiConfirmDialogSetupPhase`, `UiConfirmDialogMainPhase`, `UiConfirmDialogExitPhase`), then
   `UiScreenUpdateCommon` (removes the task once `closeRequested` `+0x4c` is set). */

typedef struct ConfirmPhaseEntry {
  s16 thisAdjust; /* +0 */
  s16 vtIndex;    /* +2: nonzero = virtual, index into the vtable */
  void *fn;       /* +4: function, or vtable offset when virtual */
} ConfirmPhaseEntry;

void UiConfirmDialogUpdate(CoreTask *task)

{
  UiScreen *screen = (UiScreen *)task;
  u32 phase = screen->phase;

  if ((s32)phase >= 0 && phase < 4) {
    const ConfirmPhaseEntry *e = (const ConfirmPhaseEntry *)g_uiConfirmDialogPhaseTable + phase;
    u8 *obj = (u8 *)task + e->thisAdjust;
    void (*fn)(void *) = (void (*)(void *))e->fn;
    if (e->vtIndex != 0) {
      const VtblEntry *v = (const VtblEntry *)(*(u8 **)(obj + (intptr_t)e->fn)) + e->vtIndex;
      fn = (void (*)(void *))v->fn;
      obj += v->delta;
    }
    fn(obj);
  }
  UiScreenUpdateCommon(screen);
}
