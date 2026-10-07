// bdc 0x0884d82c BtlMainCheckLose
#include "bdc.h"

/* Lose check of `BtlMainJudgeOutcome`: when battle rule mode `g_scriptGlobalVars[8]` is not 1
   and profile word 7 (`SaveProfileGetWord`) is 1 or 2, returns whether another player reached
   the score goal (`BtlMainHasRivalReachedGoal`); otherwise returns
   `BtlMainCheckPlayerDefeated` (player's Bakugan defeated). */
int BtlMainCheckLose(BtlMain *self)
{
    s32 scoreMode;

    if (g_scriptGlobalVars[8] == 1) {
        return BtlMainCheckPlayerDefeated(self);
    }
    scoreMode = (s32)SaveProfileGetWord(SaveGetProfile(), 7);
    if (scoreMode == 1 || scoreMode == 2) {
        return BtlMainHasRivalReachedGoal(self);
    }
    return BtlMainCheckPlayerDefeated(self);
}
