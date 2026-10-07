// bdc 0x0899aa78 UiWorldMapPhaseOptions
#include "bdc.h"

/* Phase 5 of `UiWorldMap`: opens the options screen (task 304, `UiOptionCtor`)
   keeping the shared background (`g_uiKeepSharedBg`), waits for it to close, then rebuilds the
   map state (`UiWorldMapRefreshAreaCursor`, `UiWorldMapShowButtonGuide`) and returns to
   phase 2 step 3 — or, with profile flag 0 set, to phase 4 with step 0x1d (net session) / 3,
   also writing that step into the local player's `cursorA` (other player's 0) and -1 into both
   `unkE`. Every frame it ends with the globe, area-label and jet-pose updates. */

void UiWorldMapPhaseOptions(UiScreen *screen)
{
  UiWorldMap *map = (UiWorldMap *)screen;
  int step = screen->phaseStep;

  if (step > 0 && step < 2) {
    if (!CoreTaskExists(0x130)) {
      screen->phaseStep = 2;
    }
  } else if (step == 0) {
    g_uiKeepSharedBg = 1;
    CoreTaskCreate(0x130, 100);
    screen->phaseStep++;
  } else {
    UiWorldMapRefreshAreaCursor(screen);
    UiWorldMapShowButtonGuide(screen, 1);
    screen->phase = 2;
    screen->phaseStep = 3;
    if (SaveGetProfileFlag0()) {
      int self;
      s16 next;

      screen->phase = 4;
      self = map->netSession;
      next = 0x1d;
      if (self == 0) {
        next = 3;
      }
      screen->phaseStep = next;
      map->player[self].cursorA = next;
      map->player[1 - self].cursorA = 0;
      map->player[self].unkE = -1;
      map->player[1 - self].unkE = -1;
    }
  }
  UiWorldMapUpdateGlobe(screen);
  UiWorldMapPlaceAreaLabels(screen);
  UiWorldMapUpdateJetPose(screen);
}
