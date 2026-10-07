// bdc 0x0899ace0 UiWorldMapSetResult
#include "bdc.h"

/* Sets the menu result of `UiWorldMap` when it closes (`UiSetMenuResult`). After
   a choice (`cancelFlag` = 0) stores the destination (`UiWorldMapStoreDestination`), marks the area
   visited (`UiWorldMapMarkAreaVisited`) and sets result 1 for map mode 0 (profile word 0x2b), or for
   mode 1/2 calls `SaveProfileClearPlacedHolograms`, sets word 0x2e = 1 and sets 5 (destination 0x24)
   or 3. On cancel sets 0 in mode 0, 4 in mode 1 (destination from word 0x2c) or 3 in mode 2
   (destination from word 0x33, word 0x2e = 0). Other modes set nothing. Result 2 overrides when
   profile flag 0 and profile flags 0x80 are set. */

void UiWorldMapSetResult(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  s32 mode;

  if (map->cancelFlag == 0) {
    UiWorldMapStoreDestination(screen);
    UiWorldMapMarkAreaVisited(screen);
    mode = (s32)SaveProfileGetWord(SaveGetProfile(), 0x2b);
    if (mode < 1) {
      if (mode >= 0) {
        UiSetMenuResult(screen, 1);
      }
    } else if (mode < 3) {
      SaveProfileClearPlacedHolograms();
      SaveProfileSetWord(SaveGetProfile(), 0x2e, 1);
      if (g_scriptGlobalVars[1] == 0x24) {
        UiSetMenuResult(screen, 5);
      } else {
        UiSetMenuResult(screen, 3);
      }
    }
  } else {
    mode = (s32)SaveProfileGetWord(SaveGetProfile(), 0x2b);
    if (mode < 1) {
      if (mode >= 0) {
        UiSetMenuResult(screen, 0);
      }
    } else if (mode < 2) {
      g_scriptGlobalVars[1] = (s32)SaveProfileGetWord(SaveGetProfile(), 0x2c);
      UiSetMenuResult(screen, 4);
    } else if (mode < 3) {
      SaveProfileClearPlacedHolograms();
      g_scriptGlobalVars[1] = (s32)SaveProfileGetWord(SaveGetProfile(), 0x33);
      SaveProfileSetWord(SaveGetProfile(), 0x2e, 0);
      UiSetMenuResult(screen, 3);
    }
  }
  if (SaveGetProfileFlag0() != 0 && SaveHasProfile() &&
      SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
    UiSetMenuResult(screen, 2);
  }
}
