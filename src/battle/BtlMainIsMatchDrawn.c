// bdc 0x0884c5fc BtlMainIsMatchDrawn
#include "bdc.h"

/* True when the round tallies make the multi-round match a draw. The round count is profile word
   0x1b clamped to 1..5; over those `roundResults` entries it counts the 2s (draws), 1s and 3s.
   Best of 3: two draws, or one draw with one 1 and one 3. Best of 5: two draws with one 1 and one
   3, or one draw with two 1s and two 3s, or three draws. Any other round count: at least one
   draw. */
bool BtlMainIsMatchDrawn(BtlMain *self)
{
    s32 rounds = (s32)SaveProfileGetWord(SaveGetProfile(), 0x1b);
    s32 draws;
    s32 wins;
    s32 losses;
    s32 i;

    if (rounds < 1) {
        rounds = 1;
    } else if (rounds > 5) {
        rounds = 5;
    }
    draws = 0;
    for (i = 0; i < rounds; i++) {
        if (self->roundResults[i] == 2) {
            draws++;
        }
    }
    wins = 0;
    for (i = 0; i < rounds; i++) {
        if (self->roundResults[i] == 1) {
            wins++;
        }
    }
    losses = 0;
    for (i = 0; i < rounds; i++) {
        if (self->roundResults[i] == 3) {
            losses++;
        }
    }

    if (rounds < 4) {
        if (rounds >= 3) {
            if (draws < 2) {
                return draws > 0 && wins == 1 && losses == 1;
            }
            return draws < 3;
        }
    } else if (rounds == 5) {
        if (draws < 2) {
            return draws > 0 && wins == 2 && losses == 2;
        }
        if (draws < 3) {
            return wins == 1 && losses == 1;
        }
        return draws < 4;
    }
    return draws != 0;
}
