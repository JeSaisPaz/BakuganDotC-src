// bdc 0x08a2c408 ActorIsRobot
#include "bdc.h"

/* Base actor virtual slot 12 (`+0x64`): returns 0; only the two robot classes override it with 1
   (`ActorNpcSwitchRobotIsRobot`, `ActorNpcRobotIsRobot`). */

int ActorIsRobot(Actor *self)

{
  return 0;
}

