// bdc 0x088c2b9c GameFieldPhasePause
#include "bdc.h"

/* Phase 2 of the field task (id 500, `GameFieldCtor`) (table `0x08a91b50`): waits until the pause
   menu (`UiPauseCtor`, task 410) is gone, then acts on its result (script variable 3): 4 → exit
   with request 7 (phase 3), 6 → stage 0x20 and exit, 0xe → open settings (task 301, phase 5),
   0xf → disable the world-map task, store event flags, set profile flag `0x40000000`, open the save
   task (10020, phase 7); otherwise resumes the player (`GameFieldSetPlayerPaused` 0) and returns
   to phase 1. */

void GameFieldPhasePause(CoreTask *taskBase)
{
  GameFieldTask *task = (GameFieldTask *)taskBase;
  SaveProfile *profile;

  if (CoreTaskExists(0x19a) != 0) {
    return;
  }
  switch (g_scriptGlobalVars[3]) {
  case 4:
    task->nextRequest = 7;
    task->phase = 3;
    task->subState = 0;
    break;
  case 6:
    g_scriptGlobalVars[1] = 0x20;
    task->phase = 3;
    task->subState = 0;
    break;
  case 0xe:
    CoreTaskCreate(0x12d, 100);
    task->phase = 5;
    break;
  case 0xf:
    if (task->worldMapTask != NULL) {
      CoreTaskSetFlags(task->worldMapTask, 1);
    }
    SaveProfileStoreEventFlags();
    profile = SaveGetProfile();
    SaveProfileSetFlags(profile, 0x40000000);
    CoreTaskCreate(0x2724, 100);
    task->phase = 7;
    task->subState = 0;
    break;
  default:
    GameFieldSetPlayerPaused(taskBase, false);
    task->phase = 1;
    break;
  }
}
