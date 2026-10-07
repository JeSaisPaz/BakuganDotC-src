// bdc 0x08a2bf70 GameGimmickIsBarrier
#include "bdc.h"

/* Returns 0 for the "is a barrier" gimmick class test (virtual slot 11, `+0x5c`) in GameGimmick's
   vtables. */

int GameGimmickIsBarrier(GameGimmick *gimmick)

{
  return 0;
}

