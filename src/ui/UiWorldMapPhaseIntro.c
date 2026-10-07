// bdc 0x08998958 UiWorldMapPhaseIntro
#include "bdc.h"

/* Phase 1 of `UiWorldMap`, stepped by `phaseStep`. Step 0 builds the map
   (`UiWorldMapCreateSprites`, the globe `UiWorldMapCreateMapModel`, a duration-1.0
   `UiWorldMapStartGlobeTurn`, `UiWorldMapApplyGlobeRotation`, Marucho's jet
   `UiWorldMapCreateJetModel`) and advances. Step 1 waits for the active fader to finish, then
   plays `main_start.fab` in shared-anim slot 1 (depth 100) and advances. Any other step clears
   `netSession`; without profile flag 0 it moves on to the next phase (step 0). With it, it takes the
   network path: loads the local player's settings (`UiWorldMapLoadNetSettings`), takes
   `netSession` from the NetPlay manager's local slot when there is one, jumps to phase 4 step 0,
   clears `cancelFlag`, swaps NetPlay flag 0x1000000 for 0x2000000 and resets the net character
   sync (`NetCharaResetAllSync`). */

void UiWorldMapPhaseIntro(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  s32 step = screen->phaseStep;

  if (step == 0) {
    UiWorldMapCreateSprites(screen);
    UiWorldMapCreateMapModel(screen);
    UiWorldMapStartGlobeTurn(1.0f, screen);
    UiWorldMapApplyGlobeRotation(screen);
    UiWorldMapCreateJetModel(screen);
    screen->phaseStep++;
    return;
  }
  if (step == 1) {
    if (GfxFaderIsFinished(GfxGetActiveFader())) {
      UiSharedAnimStart(100.0f, 0.0f, 0.0f, screen, (void *)"main_start.fab", 1, 0);
      screen->phaseStep++;
    }
    return;
  }

  map->netSession = 0;
  if (!SaveGetProfileFlag0()) {
    screen->phaseStep = 0;
    screen->phase++;
    return;
  }
  UiWorldMapLoadNetSettings(screen);
  if (NetPlayHasManager()) {
    map->netSession = NetPlayGetLocalSlot(NetPlayGetManager());
  }
  screen->phase = 4;
  screen->phaseStep = 0;
  map->cancelFlag = 0;
  if (NetPlayHasManager()) {
    NetPlayClearFlags(NetPlayGetManager(), 0x1000000);
    NetPlaySetFlags(NetPlayGetManager(), 0x2000000);
  }
  NetCharaResetAllSync();
}
