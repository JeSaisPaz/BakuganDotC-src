// bdc 0x0884d5c0 BtlMainRecordRoundResult
#include "bdc.h"

/* Records `result` for the current round once per battle: only while `g_btlBattleOutcome` is 0 or
   3 and `roundRecorded` is clear, sets `roundRecorded`, stores `result` in `roundResults[round]`
   with `round` = profile word 0x1e (`SaveProfileGetWord`) clamped to 0..4 as a signed value, and
   adds 1 to that word (`SaveProfileAddWord`). */

void BtlMainRecordRoundResult(BtlMain *self, int result)
{
    s32 round;

    if (g_btlBattleOutcome != 0 && g_btlBattleOutcome != 3) {
        return;
    }
    if (self->roundRecorded != 0) {
        return;
    }
    self->roundRecorded = 1;
    round = (s32)SaveProfileGetWord(SaveGetProfile(), 0x1e);
    if (round < 0) {
        round = 0;
    } else if (round > 4) {
        round = 4;
    }
    self->roundResults[round] = result;
    SaveProfileAddWord(SaveGetProfile(), 0x1e, 1);
}
