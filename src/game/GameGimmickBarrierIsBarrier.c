// bdc 0x08a2c3c8 GameGimmickBarrierIsBarrier
#include "bdc.h"

/* Returns 1 for the "is a barrier" gimmick class test (virtual slot 11, `+0x5c`) in
   GameGimmickBarrier's vtable. */

int GameGimmickBarrierIsBarrier(GameGimmickBarrier *gimmick)

{
  return 1;
}

