// bdc 0x0884c530 BtlMainIsMatchLost
#include "bdc.h"

/* True when the player's side lost a strict majority of the configured rounds: the round count is
   profile word 0x1b (`SaveProfileGetWord`) clamped to 1..5, and the number of those
   `roundResults` entries that are 3 (loss) or 2 (draw) must reach `rounds / 2 + 1`. */
bool BtlMainIsMatchLost(BtlMain *self)
{
    s32 rounds = (s32)SaveProfileGetWord(SaveGetProfile(), 0x1b);
    s32 lost = 0;
    s32 i;

    if (rounds < 1) {
        rounds = 1;
    } else if (rounds > 5) {
        rounds = 5;
    }
    for (i = 0; i < rounds; i++) {
        if (self->roundResults[i] == 3 || self->roundResults[i] == 2) {
            lost++;
        }
    }
    return lost >= rounds / 2 + 1;
}
