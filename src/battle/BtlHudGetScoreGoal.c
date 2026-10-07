// bdc 0x08833a9c BtlHudGetScoreGoal
#include "bdc.h"

/* Returns the score goal of the score battle, profile word 0xb (`SaveProfileGetWord`); -1 means
   no goal and `BtlHudPhaseBuild` then hides the goal digits. */

s32 BtlHudGetScoreGoal(BtlHud *self)
{
    (void)self;
    return (s32)SaveProfileGetWord(SaveGetProfile(), 0xb);
}
