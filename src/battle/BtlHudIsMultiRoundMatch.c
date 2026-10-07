// bdc 0x0882fc7c BtlHudIsMultiRoundMatch
#include "bdc.h"

/* Returns 1 (and sets `self->multiRound` to 1) when battle rule mode (script global entry 8) is 2
   and the round count (profile word 0x1b, clamped to 1..5) is at least 3; otherwise returns 0 and
   leaves `multiRound` unchanged. */

s32 BtlHudIsMultiRoundMatch(BtlHud *self)
{
    s32 rounds = (s32)SaveProfileGetWord(SaveGetProfile(), 0x1b);

    if (rounds < 1) {
        rounds = 1;
    } else if (rounds > 5) {
        rounds = 5;
    }
    if (g_scriptGlobalVars[8] == 2 && rounds >= 3) {
        self->multiRound = 1;
        return 1;
    }
    return 0;
}
