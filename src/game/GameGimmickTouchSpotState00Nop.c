// bdc 0x088db394 GameGimmickTouchSpotState00Nop
#include "bdc.h"

/* Empty state 0 handler, the only entry of the touch-spot gimmick's state table `0x08a96c68`
   dispatched by GameGimmickTouchSpotUpdate. */
void GameGimmickTouchSpotState00Nop(void *gimmick)
{
    (void)gimmick;
}
