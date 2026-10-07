// bdc 0x08850dbc BtlGetStoryWinStageValue
#include "bdc.h"

/* After a won story battle (`g_btlBattleOutcome` == 1, game mode script global 8 == 1) returns
   the per-stage word `g_btlStoryWinStageValues``[stage]` (stage index = script global 1); 0
   otherwise. `BtlMainPhaseFinish` stores the result in script global 14 right after
   `BtlMainRecordStageClear`. The `main` argument is unused. */
int BtlGetStoryWinStageValue(void *main)
{
    (void)main;
    if (g_btlBattleOutcome == 1 && g_scriptGlobalVars[8] == 1) {
        return g_btlStoryWinStageValues[g_scriptGlobalVars[1]];
    }
    return 0;
}
