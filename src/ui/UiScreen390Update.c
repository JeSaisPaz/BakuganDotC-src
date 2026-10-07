// bdc 0x089404bc UiScreen390Update
#include "bdc.h"

/* Per-frame update (vtable slot 2) of `UiScreen390` (terminal-use cutscene, task
   id 390): runs the phase handler selected by `phase` (`+0x28`) from the 4-entry PMF table
   `g_uiScreen390PhaseFns` (`0x08a9ced0`), then the common screen update. */

void UiScreen390Update(UiScreen *screen)

{
  int phase = screen->phase;
  u8 closing;

  if (phase >= 0 && (unsigned)phase < 4) {
    const MemberFnPtr *member = &g_uiScreen390PhaseFns[phase];
    u8 *obj = (u8 *)screen + member->delta;
    void *fn = member->pfn;

    if (member->index != 0) {
      const VtblEntry *vtbl = *(const VtblEntry **)(obj + (intptr_t)member->pfn);
      const VtblEntry *entry = &vtbl[member->index];

      fn = entry->fn;
      obj += entry->delta;
    }
    ((void (*)(void *))fn)(obj);
  }
  closing = screen->closeRequested;
  UiScreenUpdateCommon(screen);
  if (closing == 0) {
    UiScreenUpdateBg(screen);
  }
}
