// bdc 0x0890a14c UiSharedAnimSetRange
#include "bdc.h"

/* Sets the play range (`start`, `end`, passed in f12/f13) of the shared animation in `slot`
   (`g_uiSharedAnims`): stores them at fab `+0x30` / `+0x34` (`transform[0][0]`, `[0][1]`). */

void UiSharedAnimSetRange(float start, float end, void *owner, int slot)

{
  if (g_uiSharedAnims[slot] != (GfxFab *)0) {
    g_uiSharedAnims[slot]->transform[0][0] = start;
    g_uiSharedAnims[slot]->transform[0][1] = end;
  }
}
