// bdc 0x0884d77c BtlMainHasRivalReachedGoal
#include "bdc.h"

/* True when any player slot 0..3 other than `localSlot` has a score (profile word `0xe + slot`,
   `SaveProfileGetWord`) at or above the score goal (profile word 0xb, signed compare); always
   false when the goal is -1 (no goal). Used for the score battle mode; the mode itself is not
   checked here. */
bool BtlMainHasRivalReachedGoal(BtlMain *self)
{
    s32 goal = (s32)SaveProfileGetWord(SaveGetProfile(), 0xb);
    s32 slot;
    s32 score;

    for (slot = 0; slot < 4; slot++) {
        if (slot == self->localSlot) {
            continue;
        }
        score = (s32)SaveProfileGetWord(SaveGetProfile(), slot + 0xe);
        if (goal != -1 && score >= goal) {
            return true;
        }
    }
    return false;
}
