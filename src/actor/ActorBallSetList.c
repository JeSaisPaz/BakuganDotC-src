// bdc 0x088b8c94 ActorBallSetList
#include "bdc.h"

/* Resets the `ActorBall` serial counters (`g_actorBallSerial` = 0, `g_actorBallNextId` = 1) and
   sets the object list `g_actorBallList` new balls append themselves to (`ActorBallCtor`). */

void ActorBallSetList(void *list)

{
  g_actorBallSerial = 0;
  g_actorBallNextId = 1;
  g_actorBallList = list;
  return;
}
