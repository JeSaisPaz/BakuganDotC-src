// bdc 0x0884c464 BtlMainIsMatchWon
#include "bdc.h"

/* True when, over the first `rounds` entries of `roundResults` (round count = profile word 0x1b
   clamped to 1..5), the rounds recorded as won (1) or drawn (2) number at least `rounds / 2 + 1`;
   false otherwise. */

bool BtlMainIsMatchWon(BtlMain *self)
{
  s32 rounds;
  s32 i;
  s32 count;

  rounds = (s32)SaveProfileGetWord(SaveGetProfile(), 0x1b);
  if (rounds < 1) {
    rounds = 1;
  } else if (rounds > 5) {
    rounds = 5;
  }
  count = 0;
  for (i = 0; i < rounds; i++) {
    if (self->roundResults[i] == 1 || self->roundResults[i] == 2) {
      count++;
    }
  }
  return count >= rounds / 2 + 1;
}
