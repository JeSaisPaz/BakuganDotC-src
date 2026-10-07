// bdc 0x088f4c7c GameFieldCharSetQueueSwitchEvent
#include "bdc.h"

/* Appends `id` to the switch-event list `queued` (count `queuedCount`); called when a switch robot
   is shut down (`ActorNpcSwitchRobotCheckSwitchHit`). */

void GameFieldCharSetQueueSwitchEvent(GameFieldCharSet *mgr, u8 id)
{
  u8 n = mgr->queuedCount;

  mgr->queuedCount = n + 1;
  mgr->queued[n] = id;
}
