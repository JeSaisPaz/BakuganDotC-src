// bdc 0x088d9e4c GameGimmickBarrierState01FadeOut
#include "bdc.h"

/* State 1 of the barrier gimmick (table `0x08a96c10`): lowers the alpha `+0x6c` by 1/15 per frame;
   at 0 hides the attached object (`GameGimmickBarrierDisableCollider`) and returns to state 0. */

void GameGimmickBarrierState01FadeOut(GameGimmickBarrier *gimmick)

{
  gimmick->base.base.ambient[3] = gimmick->base.base.ambient[3] - 0.06666667f;
  if (gimmick->base.base.ambient[3] <= 0.0f) {
    gimmick->base.base.ambient[3] = 0.0f;
    GameGimmickBarrierDisableCollider(gimmick);
    (gimmick->base).state = 0;
  }
  return;
}

