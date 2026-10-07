// bdc 0x08a2c578 ActorNpcSlot48Nop
#include "bdc.h"

/* Empty NPC virtual slot 48 (`+0x184`), shared by the NPC, cloak, robot and guard vtables; the
   switch-robot vtable `0x08af3d04` overrides it with `ActorNpcRobotSlot48`. */

void ActorNpcSlot48Nop(ActorNpc *self)

{
  return;
}

