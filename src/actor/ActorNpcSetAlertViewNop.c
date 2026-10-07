// bdc 0x08a2c614 ActorNpcSetAlertViewNop
#include "bdc.h"

/* Default NPC virtual slot 46 (`+0x174`, view-cone alert switch called e.g. by
   `ActorNpcStateReturnHome` with 1): does nothing; guards override it with
   `ActorNpcGuardSetAlertView`. */

void ActorNpcSetAlertViewNop(ActorNpc *self, u8 alert)

{
  return;
}

