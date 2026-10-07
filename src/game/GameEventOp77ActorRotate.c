// bdc 0x088f0b64 GameEventOp77ActorRotate
#include "bdc.h"

/* Handler of event opcode 0x77 (`GameEvent470ExecCommand`): queues a rotate tween of `arg` frames
   for event actor `flag` (kind 1). */

void GameEventOp77ActorRotate(GameEvent470 *self, u8 flag, s16 arg)

{
  GameEvent470AddActorTween(self,arg,'\x01',self->actorMap[flag]);
  return;
}

