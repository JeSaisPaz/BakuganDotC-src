// bdc 0x08a2c5fc ActorNpcGuardListensForNoise
#include "bdc.h"

/* Guard/cloaked-guard override of NPC virtual slot 33 (`+0x10c`): returns 1, so
   `ActorNotifyNearestGuardOfNoise` considers the actor when routing a noise to
   `ActorNpcHearNoise`. */

int ActorNpcGuardListensForNoise(ActorNpc *self)

{
  return 1;
}

