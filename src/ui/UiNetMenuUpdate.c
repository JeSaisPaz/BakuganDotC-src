// bdc 0x0894ddec UiNetMenuUpdate
#include "bdc.h"

/* Per-frame update (vtable slot 2) of the multiplayer (ad-hoc) top menu (task id 1999): runs the
   phase handler selected by `phase` (`+0x28`) from the 4-entry PMF table `0x08a9d360`, then the
   common screen update. */

void UiNetMenuUpdate(UiScreen *screen)

{
  u8 closeRequested;
  const MemberFnPtr *member;
  u8 *self;
  void *fn;
  u32 phase;

  phase = screen->phase;
  if ((-1 < (int)phase) && (phase < 4)) {
    member = &g_uiNetMenuPhaseFns[phase];
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
  if (closeRequested == 0) {
    UiScreenUpdateBg(screen);
  }
  return;
}
