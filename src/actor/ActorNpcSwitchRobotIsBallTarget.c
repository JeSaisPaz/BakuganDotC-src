// bdc 0x08a2c690 ActorNpcSwitchRobotIsBallTarget
#include "bdc.h"

/* Switch-robot override of actor virtual slot 11 (`+0x5c`): returns 1 — the ball can hit and home
   on it (`ActorBallCheckHit` → `ActorNpcSwitchRobotCheckSwitchHit`). */

int ActorNpcSwitchRobotIsBallTarget(ActorNpc *self)

{
  return 1;
}

