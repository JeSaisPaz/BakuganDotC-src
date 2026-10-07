// bdc 0x0890cfb4 UiLoadingSyncTipScroll
#include "bdc.h"

/* Propagates the tip text offset `+0x1c0` of the now-loading screen (task 10100 / 0x2774, 0x240
   bytes, vtable `0x08af47dc`, `UiLoadingCtor`; shared UI objects `0x08ac0e80`) to every line of
   the message box's printer (`+0xbc` of each node, via `UiTextBoxGetPrinter`) when it changed
   (`+0x1c8`), or zeroes it for tips whose kind (`0x08a9b060[tip].kind`) is not 1/2. */

void UiLoadingSyncTipScroll(UiLoading *self)

{
  GfxSprite *node;
  float scroll;

  node = NULL;
  if (g_uiLoadingShared->box != NULL) {
    node = ((UiTextPrinter *)UiTextBoxGetPrinter(g_uiLoadingShared->box))->glyphs;
  }
  if (g_loadingTipKinds[self->theme][0] < 1 || 2 < g_loadingTipKinds[self->theme][0]) {
    for (; node != NULL; node = node->next) {
      node->alpha = 0.0f;
    }
  }
  else {
    scroll = self->tipScroll;
    if (self->tipScrollEnd != scroll) {
      self->tipScrollEnd = scroll;
      for (; node != NULL; node = node->next) {
        node->alpha = scroll;
      }
    }
  }
  return;
}
