// bdc 0x088c1ddc GameFieldRestoreStage20EventWord
#include "bdc.h"

/* On stage 0x14 (script variable 1) copies the saved word `g_gameTransitionParam` back into the
   event-state halfword at `g_gameEventFlags` `+6` (story event block, see `GameEventStateClear`);
   `GameFieldCommitStageProgress` saves it. */

void GameFieldRestoreStage20EventWord(void)
{
  if (g_scriptGlobalVars[1] == 0x14) {
    *(s16 *)&g_gameEventFlags[6] = g_gameTransitionParam;
  }
}
