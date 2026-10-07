// bdc 0x0899c6dc UiWorldMapStageListDone
#include "bdc.h"

/* Advances the fade tweens of all stage-list sprites of `UiWorldMap` (0x32..0x34,
   0x36, 0x3a..0x48, 0x37..0x39, 0x49..0x5a in that order; `UiWorldMapStageSpriteDone`) and
   re-places the stage texts (`UiWorldMapPlaceStageTexts`); returns true when at least one
   sprite tween reported finished (the u8 sum of the results is nonzero). */

bool UiWorldMapStageListDone(UiScreen *screen, u8 out)
{
  u8 count = 0;
  int i;

  for (i = 0x32; i < 0x35; i++) {
    count += UiWorldMapStageSpriteDone(screen, out, i);
  }
  for (i = 0x36; i < 0x37; i++) {
    count += UiWorldMapStageSpriteDone(screen, out, i);
  }
  for (i = 0x3a; i < 0x43; i++) {
    count += UiWorldMapStageSpriteDone(screen, out, i);
  }
  for (i = 0x43; i < 0x49; i++) {
    count += UiWorldMapStageSpriteDone(screen, out, i);
  }
  for (i = 0x37; i < 0x3a; i++) {
    count += UiWorldMapStageSpriteDone(screen, out, i);
  }
  for (i = 0x49; i < 0x58; i++) {
    count += UiWorldMapStageSpriteDone(screen, out, i);
  }
  for (i = 0x58; i < 0x5b; i++) {
    count += UiWorldMapStageSpriteDone(screen, out, i);
  }
  UiWorldMapPlaceStageTexts(screen);
  return count != 0;
}
