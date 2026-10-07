// bdc 0x0884d1cc BtlMainIsScoreGoalReached
#include "bdc.h"

/* Ranked-mode win test: true when the score goal (profile word 0xb) is not -1 and the local
   player's score (profile word 0xe + localSlot) is at least the goal (signed compare), or when
   profile word 2 (remaining time) is 0 and the local player is in first place. */
bool BtlMainIsScoreGoalReached(BtlMain *self)
{
    bool reached;
    s32 goal;
    s32 score;

    reached = false;
    goal = (s32)SaveProfileGetWord(SaveGetProfile(), 0xb);
    score = (s32)SaveProfileGetWord(SaveGetProfile(), self->localSlot + 0xe);
    if (goal != -1 && goal <= score) {
        reached = true;
    }
    if (SaveProfileGetWord(SaveGetProfile(), 2) == 0 && BtlMainGetPlayerPlacing(self, -1) == 0) {
        reached = true;
    }
    return reached;
}
