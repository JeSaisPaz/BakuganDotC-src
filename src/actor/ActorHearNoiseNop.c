// bdc 0x08a2c430 ActorHearNoiseNop
#include "bdc.h"

/* Base actor virtual slot 17 (`+0x8c`, noise notification): does nothing; the NPC/guard classes
   override it with `ActorNpcHearNoise`. */

void ActorHearNoiseNop(Actor *self, float *pos, s32 level)

{
  return;
}

