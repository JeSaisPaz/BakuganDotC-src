// bdc 0x0899ec64 UiWorldMapCheckOptionsButton
#include "bdc.h"

/* Returns 1 when the square button (pad pressed `0x8000`) is pressed on
   `UiWorldMap` while its guide sprite 0x26 is visible (opens the options, phase
   5), else 0. */

int UiWorldMapCheckOptionsButton(UiScreen *screen)
{
  GfxSprite **sprites = (GfxSprite **)screen->data;

  if (((s8)(screen->pad->pressed >> 8) & 0x80) != 0 && (sprites[0x26]->flags & 1) != 0) {
    return 1;
  }
  return 0;
}
