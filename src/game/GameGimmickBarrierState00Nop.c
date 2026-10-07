// bdc 0x088d9e44 GameGimmickBarrierState00Nop
#include "bdc.h"

/* Empty state 0 (idle) handler of the barrier gimmick (`GameGimmickBarrierCtor`); state 1 is
   `GameGimmickBarrierState01FadeOut`. Does nothing. */
void GameGimmickBarrierState00Nop(void *gimmick)
{
    (void)gimmick;
}
