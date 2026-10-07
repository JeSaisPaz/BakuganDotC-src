// bdc 0x08a2c698 ActorNpcSwitchRobotIsRobot
#include "bdc.h"

/* Switch-robot override of actor virtual slot 12 (`+0x64`): returns 1. */

int ActorNpcSwitchRobotIsRobot(ActorNpc *self)

{
  return 1;
}

