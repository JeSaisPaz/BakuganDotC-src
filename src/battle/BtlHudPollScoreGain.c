// bdc 0x088335a0 BtlHudPollScoreGain
#include "bdc.h"

/* Detects a change in `player`'s score (`BtlHudGetPlayerScore`): when the pending gain
   `gainPending[player]` is below 99 and the score differs from the last seen value
   `scoreSeen[player]`, stores the new score, sets the pending gain to
   `new - old + gainShown[player]` capped at 99 (lowering `scoreSeen` by the excess), and returns 1;
   otherwise 0. */
s32 BtlHudPollScoreGain(BtlHud *self, s32 player)
{
    s32 score = BtlHudGetPlayerScore(self, player);
    s32 old;
    s32 gain;

    if (self->gainPending[player] >= 99) {
        return 0;
    }
    old = self->scoreSeen[player];
    if (old == score) {
        return 0;
    }
    gain = (score - old) + self->gainShown[player];
    self->scoreSeen[player] = score;
    self->gainPending[player] = gain;
    if (gain >= 100) {
        self->scoreSeen[player] = (self->scoreSeen[player] - gain) + 99;
        self->gainPending[player] = 99;
    }
    return 1;
}
