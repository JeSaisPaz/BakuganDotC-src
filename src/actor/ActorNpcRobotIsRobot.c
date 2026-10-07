// bdc 0x08a2c6a0 ActorNpcRobotIsRobot
#include "bdc.h"

/* Robot override of actor virtual slot 12 (`+0x64`): returns 1. */

int ActorNpcRobotIsRobot(ActorNpc *self)

{
  return 1;
}

