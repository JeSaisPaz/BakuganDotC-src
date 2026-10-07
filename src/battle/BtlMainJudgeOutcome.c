// bdc 0x0884da14 BtlMainJudgeOutcome
#include "bdc.h"

/* Decides the battle outcome: returns 3 when `checkTimeUp` is set and profile word 2 is 0.
   Otherwise, if BtlMainCheckWin: round result 2 and outcome 4 when BtlMainIsDraw, else round
   result 1 and outcome 1. Else if BtlMainCheckLose: draw gives 2/4, else round result 3 and
   outcome 2. Else, when `checkTimeUp` is clear, profile word 2 is 0, BtlMainIsBehindOnTimeUp(self,
   false) holds and it is not a draw: round result 3, outcome 2. Otherwise, when
   SaveGetProfileFlag0 is set: with a profile present whose flags have 0x80, sets
   g_scriptGlobalVars[3] = 6 and returns 6; else if the profile flags have 0x800, sets
   g_scriptGlobalVars[3] = 10 and returns 7. Returns 0 in every remaining case. */

int BtlMainJudgeOutcome(BtlMain *self, bool checkTimeUp)
{
    if (checkTimeUp) {
        if (SaveProfileGetWord(SaveGetProfile(), 2) == 0) {
            return 3;
        }
    }
    if (BtlMainCheckWin(self) != 0) {
        if (BtlMainIsDraw(self)) {
            BtlMainRecordRoundResult(self, 2);
            return 4;
        }
        BtlMainRecordRoundResult(self, 1);
        return 1;
    }
    if (BtlMainCheckLose(self) != 0) {
        if (BtlMainIsDraw(self)) {
            BtlMainRecordRoundResult(self, 2);
            return 4;
        }
        BtlMainRecordRoundResult(self, 3);
        return 2;
    }
    if (!checkTimeUp) {
        if (SaveProfileGetWord(SaveGetProfile(), 2) == 0 && BtlMainIsBehindOnTimeUp(self, false) &&
            !BtlMainIsDraw(self)) {
            BtlMainRecordRoundResult(self, 3);
            return 2;
        }
    }
    if (SaveGetProfileFlag0() != 0) {
        if (SaveHasProfile() && SaveProfileHasFlags(SaveGetProfile(), 0x80)) {
            g_scriptGlobalVars[3] = 6;
            return 6;
        }
        if (SaveProfileHasFlags(SaveGetProfile(), 0x800)) {
            g_scriptGlobalVars[3] = 10;
            return 7;
        }
    }
    return 0;
}
