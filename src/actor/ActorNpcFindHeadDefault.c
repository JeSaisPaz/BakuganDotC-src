// bdc 0x08a2c604 ActorNpcFindHeadDefault
#include "bdc.h"

/* Default NPC virtual slot 32 (`+0x104`, head-node lookup run by `ActorNpcUpdate`): does nothing
   and returns 1; the robot and guard classes override it (`ActorNpcRobotFindHead`,
   `ActorNpcGuardFindHead`). */

bool ActorNpcFindHeadDefault(ActorNpc *self)

{
  return true;
}

