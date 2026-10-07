// bdc 0x088a9580 ActorStageObjGetShakeOffset
#include "bdc.h"

/* Returns entry `step & 0x1f` of the 32-entry s16 shake table `0x08abd5c0` (copied to the stack):
   the X/Z jitter applied while a hit object shakes. */

int ActorStageObjGetShakeOffset(ActorStageObjBase *self, u32 step)

{
  short shakeTable [32];
  
  memcpy(shakeTable,g_actorStageObjShakeTable,0x40);
  return (int)shakeTable[step & 0x1f];
}

