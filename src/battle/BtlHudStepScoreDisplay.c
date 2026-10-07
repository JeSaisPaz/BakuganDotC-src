// bdc 0x08833990 BtlHudStepScoreDisplay
#include "bdc.h"

/* Returns the score shown for `player` (`scoreShown[player]`), stepping it by 1 toward the last
   seen score `scoreSeen[player]` while the count-up flag `scoreCountUp[player]` is set; once it
   reaches (or passes) that score it is set to it, the flag is cleared and the done flag
   `scoreCountDone[player]` is set (read by `BtlHudUpdateScoreGainPopup` state 7). Returns -1,
   touching nothing, when `BtlHudGetPlayerScore` returns -1. */
s32 BtlHudStepScoreDisplay(BtlHud *self, s32 player)
{
    s32 shown;

    if (BtlHudGetPlayerScore(self, player) == -1) {
        return -1;
    }
    if (self->scoreCountUp[player] != 0) {
        shown = self->scoreShown[player] + 1;
        self->scoreShown[player] = shown;
        if (shown >= self->scoreSeen[player]) {
            self->scoreShown[player] = self->scoreSeen[player];
            self->scoreCountUp[player] = 0;
            self->scoreCountDone[player] = 1;
        }
    }
    return self->scoreShown[player];
}
