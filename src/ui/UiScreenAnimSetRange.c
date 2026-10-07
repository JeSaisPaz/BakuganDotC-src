// bdc 0x08909f4c UiScreenAnimSetRange
#include "bdc.h"

/* Sets the screen's animation (`GfxFab *` in `slot` of `bgData`) translation row: `transform[3][0] = start`, `transform[3][1] = end`
   (fab `+0x60`/`+0x64`; the name "range" comes from the caller). */

void UiScreenAnimSetRange(UiScreen *screen, int slot, float start, float end)

{
  GfxFab **fabs = (GfxFab **)screen->bgData;

  if (fabs[slot] != NULL) {
    fabs[slot]->transform[3][0] = start;
    ((GfxFab **)screen->bgData)[slot]->transform[3][1] = end;
  }
}
