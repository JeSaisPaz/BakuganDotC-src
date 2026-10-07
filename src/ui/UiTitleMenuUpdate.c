// bdc 0x0895032c UiTitleMenuUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the title/system menu screen (task id 1000): runs the phase
   handler selected by `phase` (`+0x28`) from the 4-entry PMF table `g_uiTitleMenuPhaseFns`, then the common
   screen update. */

void UiTitleMenuUpdate(UiScreen *screen)

{
  u8 closeRequested;
  const MemberFnPtr *member;
  u8 *self;
  void *fn;
  u32 phase;

  phase = screen->phase;
  if ((-1 < (int)phase) && (phase < 4)) {
    member = &g_uiTitleMenuPhaseFns[phase];
    self = (u8 *)screen + member->delta;
    fn = member->pfn;
    if (member->index != 0) {
      const VtblEntry *vtbl = *(const VtblEntry **)(self + (intptr_t)member->pfn);
      const VtblEntry *entry = &vtbl[member->index];

      fn = entry->fn;
      self += entry->delta;
    }
    ((void (*)(void *))fn)(self);
  }
  closeRequested = screen->closeRequested;
  UiScreenUpdateCommon(screen);
  if (closeRequested == 0 && screen->bgData != (void *)0x0) {
    GfxFabListUpdate(&screen->bgAnimList);
  }
  return;
}
