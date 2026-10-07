// bdc 0x0884d0f4 BtlMainGetPlayerPlacing
#include "bdc.h"

/* Placing of player `slot` (-1 = the main task's `localSlot`, `+0x548`) in a ranked battle: the
   number of the 4 players whose profile score word `0xe + i` is higher than its own (signed
   compare; 0 = first). Returns 4 unless profile word 7 is 1 or 2. */
int BtlMainGetPlayerPlacing(BtlMain *self, int slot)
{
    s32 mode;
    s32 score;
    s32 placing = 0;
    s32 i;

    mode = (s32)SaveProfileGetWord(SaveGetProfile(), 7);
    if (mode <= 0 || mode >= 3) {
        return 4;
    }
    if (slot == -1) {
        slot = self->localSlot;
    }
    score = (s32)SaveProfileGetWord(SaveGetProfile(), slot + 0xe);
    for (i = 0; i < 4; i++) {
        if (score < (s32)SaveProfileGetWord(SaveGetProfile(), i + 0xe)) {
            placing++;
        }
    }
    return placing;
}
