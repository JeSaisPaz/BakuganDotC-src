// bdc 0x08833afc BtlHudGetScoreCounterValue
#include "bdc.h"

/* Returns the current value of score-board digit counter `counter` (0..13, see
   `BtlHudSetScoreDigits`): 0-3 the displayed score of players 0-3
   (`BtlHudStepScoreDisplay0`..`BtlHudStepScoreDisplay3`), 4-7 the shown gain of players 0-3,
   8 and 9 the score goal (`BtlHudGetScoreGoal`), 10 and 12 the displayed score of players 0/1
   again (`BtlHudStepScoreDisplay0Alt`/`BtlHudStepScoreDisplay1Alt`), 11 and 13 the gains of
   players 0/1; 0 for anything else (negative included). */

s32 BtlHudGetScoreCounterValue(BtlHud *self, s32 counter)
{
    switch (counter) {
    case 0:
        return BtlHudStepScoreDisplay0(self);
    case 1:
        return BtlHudStepScoreDisplay1(self);
    case 2:
        return BtlHudStepScoreDisplay2(self);
    case 3:
        return BtlHudStepScoreDisplay3(self);
    case 4:
    case 11:
        return self->gainShown[0];
    case 5:
    case 13:
        return self->gainShown[1];
    case 6:
        return self->gainShown[2];
    case 7:
        return self->gainShown[3];
    case 8:
    case 9:
        return BtlHudGetScoreGoal(self);
    case 10:
        return BtlHudStepScoreDisplay0Alt(self);
    case 12:
        return BtlHudStepScoreDisplay1Alt(self);
    default:
        return 0;
    }
}
