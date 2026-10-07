// bdc 0x0899b700 UiWorldMapPreviewDone
#include "bdc.h"

/* Advances the tweens of the four area-preview sprites 0x1a, 0x1b, 0x20, 0x21 of
   `UiWorldMap` (`UiWorldMapPreviewSpriteDone`); returns true when finished. */

bool UiWorldMapPreviewDone(UiScreen *screen, u8 out)

{
  u8 sum;

  sum = UiWorldMapPreviewSpriteDone(screen, out, 0x1b);
  sum = sum + UiWorldMapPreviewSpriteDone(screen, out, 0x1a);
  sum = sum + UiWorldMapPreviewSpriteDone(screen, out, 0x20);
  sum = sum + UiWorldMapPreviewSpriteDone(screen, out, 0x21);
  return sum != 0;
}
