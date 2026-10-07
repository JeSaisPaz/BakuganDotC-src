// bdc 0x0882bb24 ActorCrystalSpawnTaskUpdate
#include "bdc.h"

/* Update of the crystal spawn task (id 105, `ActorCrystalSpawnTaskCtor`): while the battle task
   exists (`BtlCameraTaskExists`) and the battle is not locked (`g_btlBattleOutcome` `== 0`) runs
   `ActorCrystalSpawnTaskStep`; otherwise removes itself (`CoreTaskRemove`). */

void ActorCrystalSpawnTaskUpdate(CoreTask *task)
{
    bool run = false;

    if (BtlCameraTaskExists() != 0 && g_btlBattleOutcome == 0)
        run = true;
    if (run)
        ActorCrystalSpawnTaskStep(task);
    else
        CoreTaskRemove(task, true);
}
