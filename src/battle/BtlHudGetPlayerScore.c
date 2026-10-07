// bdc 0x08833528 BtlHudGetPlayerScore
#include "bdc.h"

/* Returns the score of `player` (clamped to 0..3) in the score battle: profile word `0xe + player`
   (`SaveProfileGetWord` on `SaveGetProfile`). */
s32 BtlHudGetPlayerScore(BtlHud *self, s32 player)
{
    (void)self;
    if (player < 0) {
        player = 0;
    } else if (player > 3) {
        player = 3;
    }
    return SaveProfileGetWord(SaveGetProfile(), player + 0xe);
}
