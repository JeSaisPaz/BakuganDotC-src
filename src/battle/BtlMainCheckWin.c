// bdc 0x0884d280 BtlMainCheckWin
#include "bdc.h"

/* Win check of `BtlMainJudgeOutcome`. In battle rule mode 1 (script global variable 8): unless
   `fieldFlags[3]` is set, wins when every enemy is defeated (`BtlCheckEnemiesDefeated`, starting
   the finish cinematic) and then sets the stage's clear bit
   `g_btlStageClearBitTable``[stage]` (stage = script global variable 1; -1 = none) in
   `g_scriptGlobalBits` (`CoreBitsetSet`). In any other rule mode: profile word 7 of 1 or 2
   (score mode) returns `BtlMainIsScoreGoalReached`, any other value
   `BtlCheckEnemiesDefeated` (with the cinematic). */

int BtlMainCheckWin(BtlMain *self)
{
    int won = 0;
    int scoreMode;
    s32 bit;

    if (g_scriptGlobalVars[8] == 1) {
        if (self->fieldFlags[3] == 0) {
            won = BtlCheckEnemiesDefeated(self, true);
            if (won != 0) {
                bit = g_btlStageClearBitTable[g_scriptGlobalVars[1]];
                if (bit != -1) {
                    CoreBitsetSet(bit, g_scriptGlobalBits);
                }
            }
        }
        return won;
    }
    scoreMode = SaveProfileGetWord(SaveGetProfile(), 7);
    if (scoreMode == 1 || scoreMode == 2) {
        return BtlMainIsScoreGoalReached(self);
    }
    return BtlCheckEnemiesDefeated(self, true);
}
