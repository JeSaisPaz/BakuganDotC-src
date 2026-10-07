// bdc 0x088df0f4 ActorAddSoundEmitter
#include "bdc.h"

/* Adds an emitter to the actor's sound object (`+0x12c`, `SndObjectAddEmitter`). */

void ActorAddSoundEmitter(void *actor, s32 soundId, u8 a, u8 b)

{
  SndObjectAddEmitter(((Actor *)actor)->base.sound,soundId,a,b);
  return;
}

