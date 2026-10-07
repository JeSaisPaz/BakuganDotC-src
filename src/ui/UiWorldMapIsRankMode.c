// bdc 0x08996408 UiWorldMapIsRankMode
#include "bdc.h"

/* Returns whether the world-map mode (profile word 0x2b, set by `SaveProfileSetMapMode`) is
   non-zero. Mode 0 is the story world map; modes 1/2 make `UiWorldMap` a stage
   picker: choosing an area opens a stage list with per-stage evaluation ranks (`"hyouka_moji_*"`,
   profile records at `+0x96`, `UiWorldMapLoadStageRanks`) instead of the story message/confirm
   flow, and the area/flag tables come from profile `+0x8f`/`+0x462` instead of `+0x8b`. */

bool UiWorldMapIsRankMode(UiScreen *screen)
{
  return SaveProfileGetWord(SaveGetProfile(), 0x2b) != 0;
}
