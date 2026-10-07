// bdc 0x0884cfc4 BtlMainSetBgmState
#include "bdc.h"

/* Sets the battle BGM state `bgmState` to `state`; nothing happens when it is unchanged. While no
   battle outcome is set (`g_btlBattleOutcome` == 0, sampled on entry), leaving state 1 queues a
   0.5-second fade-out of BGM channel 0 (`SndBgmQueueStop`) and entering state 0 queues
   `BtlMainGetBgmId` on channel 0 with `loop` 1, `flagB` 0 (`SndBgmQueuePlay`). */

void BtlMainSetBgmState(BtlMain *self, int state)
{
    bool noOutcome;
    int prev;

    prev = self->bgmState;
    noOutcome = g_btlBattleOutcome == 0;
    if (prev == state) {
        return;
    }
    if (prev == 1 && noOutcome) {
        SndBgmQueueStop(0.5f, 0);
    }
    self->bgmState = state;
    if (state == 0 && noOutcome) {
        SndBgmQueuePlay(0, BtlMainGetBgmId(self), 1, 0);
    }
}
