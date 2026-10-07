// bdc 0x088da5c4 GameGimmickTriggerZoneState00Nop
#include "bdc.h"

/* Empty state 0 handler, the only entry of the trigger-zone gimmick's state table `0x08a96c48`
   dispatched by `GameGimmickTriggerZoneUpdate`. */

void GameGimmickTriggerZoneState00Nop(void *gimmick)
{
    (void)gimmick;
}
