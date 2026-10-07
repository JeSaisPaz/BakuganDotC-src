// bdc 0x0890a318 UiScreenAnimStart
#include "bdc.h"

/* Starts an animation on a screen: when the array `screen+0x50` exists and `slot` is free, creates
   the player (`UiScreenAnimCreate`), sets its loop flag `+0x9c = loop` and `+0x98 = mode`
   (depth), and its range (`UiScreenAnimSetRange``(screen, slot, start, end)`). */

void UiScreenAnimStart(UiScreen *screen, void *data, int slot, u8 loop, float mode, float start,
                       float end)

{
  GfxFab **fabs = (GfxFab **)screen->bgData;

  if (fabs != NULL && fabs[slot] == NULL) {
    UiScreenAnimCreate(screen, data, slot);
    ((GfxFab **)screen->bgData)[slot]->loop = loop;
    ((GfxFab **)screen->bgData)[slot]->depth = mode;
    UiScreenAnimSetRange(screen, slot, start, end);
  }
}
