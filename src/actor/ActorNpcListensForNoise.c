// bdc 0x08a2c60c ActorNpcListensForNoise
#include "bdc.h"

/* Default NPC virtual slot 33 (`+0x10c`): returns 0, so `ActorNotifyNearestGuardOfNoise` skips
   plain NPCs and robots; guards override it with `ActorNpcGuardListensForNoise`. */

int ActorNpcListensForNoise(ActorNpc *self)

{
  return 0;
}

