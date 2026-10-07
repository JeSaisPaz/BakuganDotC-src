// bdc 0x08a2c580 ActorNpcUpdateFeetNop
#include "bdc.h"

/* Empty NPC virtual slot 49 (`+0x18c`), shared by the NPC, cloak, robot and guard vtables; the
   switch robot overrides it with `ActorNpcSwitchRobotUpdateFeet`. */

void ActorNpcUpdateFeetNop(ActorNpc *self)

{
  return;
}

