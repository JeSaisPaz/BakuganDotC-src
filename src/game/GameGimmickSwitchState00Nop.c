// bdc 0x088dbba8 GameGimmickSwitchState00Nop
#include "bdc.h"

/* Empty state 0 handler, the only entry of the switch gimmick's state table `0x08a96c70`
   dispatched by `GameGimmickSwitchUpdate`. */

void GameGimmickSwitchState00Nop(void *gimmick)
{
    (void)gimmick;
}
