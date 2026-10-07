// bdc 0x0884c7f0 BtlMainIsMatchUndecided
#include "bdc.h"

/* True in a multi-round battle (battle rule mode selector, script global variable entry 8 of
   `g_scriptGlobalVars`, is 2 and the round count, profile word 0x1b clamped to 1..5, is at
   least 3) while none of `BtlMainIsMatchWon`, `BtlMainIsMatchLost`, `BtlMainIsMatchDrawn`
   holds (checked in that order, stopping at the first true one), i.e. another round must be
   fought; false otherwise. */

bool BtlMainIsMatchUndecided(BtlMain *self)
{
  s32 rounds;
  bool undecided;

  undecided = false;
  rounds = (s32)SaveProfileGetWord(SaveGetProfile(), 0x1b);
  if (rounds < 1) {
    rounds = 1;
  } else if (rounds > 5) {
    rounds = 5;
  }
  if (g_scriptGlobalVars[8] == 2 && rounds >= 3) {
    undecided = true;
    if (BtlMainIsMatchWon(self)) {
      undecided = false;
    } else if (BtlMainIsMatchLost(self)) {
      undecided = false;
    } else if (BtlMainIsMatchDrawn(self)) {
      undecided = false;
    }
  }
  return undecided;
}
