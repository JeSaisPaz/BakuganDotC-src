// bdc 0x088c2e1c GameFieldPhaseSettings
#include "bdc.h"

/* Phase 5 of the field task (id 500, `GameFieldCtor`): once the settings screen
   (`UiPauseSettingsCtor`, task 301) is gone, reopens the pause menu with the saved mode `+0x6a4`
   (`GameFieldOpenPauseMenu`). */

void GameFieldPhaseSettings(CoreTask *task)

{
  if (CoreTaskExists(0x12d) == 0) {
    GameFieldOpenPauseMenu(task,((GameFieldTask *)task)->pauseMode);
  }
  return;
}
