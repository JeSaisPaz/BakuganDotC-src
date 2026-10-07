// bdc 0x08a2c400 ActorIsBallTarget
#include "bdc.h"

/* Base actor virtual slot 11 (`+0x5c`): returns 0 — the actor cannot be hit or homed on by the
   thrown ball; only the switch robot overrides it (`ActorNpcSwitchRobotIsBallTarget`). */

int ActorIsBallTarget(Actor *self)

{
  return 0;
}

