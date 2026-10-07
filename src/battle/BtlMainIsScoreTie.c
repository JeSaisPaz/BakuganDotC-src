// bdc 0x0884d38c BtlMainIsScoreTie
#include "bdc.h"

/* Score-battle tie test: counts the players `n` whose profile word 3..6 is positive, then returns
   true when the scores in profile words 0xe .. 0xe + n - 1 all equal word 0xe (the first `n`
   score slots are compared, whichever players are participating); false when `n` is 0. Does not
   check the game mode itself (its caller `BtlMainIsDraw` does). */

bool BtlMainIsScoreTie(void)
{
  s32 i;
  s32 players;
  s32 same;
  u32 firstScore;

  players = 0;
  for (i = 0; i < 4; i++) {
    if ((s32)SaveProfileGetWord(SaveGetProfile(), i + 3) > 0) {
      players++;
    }
  }
  firstScore = SaveProfileGetWord(SaveGetProfile(), 0xe);
  same = 1;
  for (i = 1; i < players; i++) {
    if (firstScore == SaveProfileGetWord(SaveGetProfile(), i + 0xe)) {
      same++;
    }
  }
  return players == same;
}
