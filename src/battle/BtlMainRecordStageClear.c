// bdc 0x0884cedc BtlMainRecordStageClear
#include "bdc.h"

/* Story-battle clear bookkeeping, called by `BtlMainPhaseFinish` after a won game-mode-1 battle:
   when the current stage's profile state (`SaveProfileData` `stageStates[stage]`, stage = script
   global 1 of `g_scriptGlobalVars`) is 2 and `BtlGetExitScene` gives code 1, records the clear
   with `SaveProfileMarkStageCleared`; it then calls `BtlGetExitScene` once more and drops the
   result (only its `SaveProfileSetBattleOutcome` side effect for code 5 remains). */

void BtlMainRecordStageClear(BtlMain *self)
{
    SaveProfile *profile;

    profile = (SaveProfile *)SaveGetProfile();
    if (profile->data->stageStates[g_scriptGlobalVars[1]] == 2 && BtlGetExitScene(self) == 1) {
        SaveProfileMarkStageCleared();
    }
    BtlGetExitScene(self);
}
