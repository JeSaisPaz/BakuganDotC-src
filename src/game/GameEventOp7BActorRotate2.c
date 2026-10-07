// bdc 0x088f0d8c GameEventOp7BActorRotate2
#include "bdc.h"

/* Handler of event opcode 0x7b (`GameEvent470ExecCommand`): same as `GameEventOp77ActorRotate`.
    */

void GameEventOp7BActorRotate2(GameEvent470 *self, u8 flag, s16 arg)

{
  GameEvent470AddActorTween(self,arg,'\x01',self->actorMap[flag]);
  return;
}

