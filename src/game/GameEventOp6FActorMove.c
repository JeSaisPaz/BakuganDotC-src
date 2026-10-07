// bdc 0x088f06d8 GameEventOp6FActorMove
#include "bdc.h"

/* Handler of event opcode 0x6f (`GameEvent470ExecCommand`): queues a move tween of `arg` frames
   for event actor `flag` (`GameEvent470AddActorTween` kind 0). */

void GameEventOp6FActorMove(GameEvent470 *self, u8 flag, s16 arg)

{
  GameEvent470AddActorTween(self,arg,'\0',self->actorMap[flag]);
  return;
}

