// bdc 0x088f09c0 GameEventOp73ActorMove2
#include "bdc.h"

/* Handler of event opcode 0x73 (`GameEvent470ExecCommand`): same as `GameEventOp6FActorMove`.
    */

void GameEventOp73ActorMove2(GameEvent470 *self, u8 flag, s16 arg)

{
  GameEvent470AddActorTween(self,arg,'\0',self->actorMap[flag]);
  return;
}

