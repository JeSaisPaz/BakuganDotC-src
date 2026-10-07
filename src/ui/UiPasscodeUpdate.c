// bdc 0x0893d83c UiPasscodeUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the passcode (symbol sequence) puzzle screen (task id 374):
   runs the phase handler selected by `phase` (`+0x28`) from the 4-entry PMF table
   `g_uiPasscodePhaseTable`, then `UiScreenUpdateCommon` and, unless a close was requested,
   `UiScreenUpdateBg`. */

typedef struct PasscodePhaseEntry {
  s16 thisAdjust; /* +0 */
  s16 vtIndex;    /* +2: nonzero = virtual, index into the vtable */
  void *fn;       /* +4: function, or vtable offset when virtual */
} PasscodePhaseEntry;

void UiPasscodeUpdate(UiScreen *screen)

{
  u32 phase = screen->phase;
  u8 closeRequested;

  if ((s32)phase >= 0 && phase < 4) {
    const PasscodePhaseEntry *e = (const PasscodePhaseEntry *)g_uiPasscodePhaseTable + phase;
    u8 *obj = (u8 *)screen + e->thisAdjust;
    void (*fn)(void *) = (void (*)(void *))e->fn;
    if (e->vtIndex != 0) {
      const VtblEntry *v = (const VtblEntry *)(*(u8 **)(obj + (intptr_t)e->fn)) + e->vtIndex;
      fn = (void (*)(void *))v->fn;
      obj += v->delta;
    }
    fn(obj);
  }
  closeRequested = screen->closeRequested;
  UiScreenUpdateCommon(screen);
  if (closeRequested == 0) {
    UiScreenUpdateBg(screen);
  }
}
