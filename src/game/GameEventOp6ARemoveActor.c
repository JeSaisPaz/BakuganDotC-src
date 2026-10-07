// bdc 0x088f0488 GameEventOp6ARemoveActor
#include "bdc.h"

/* Handler of event opcode 0x6a (`GameEvent470ExecCommand`): removes character-set entry `flag`
   from the field (`GameFieldPartyRemove` -> `GameFieldCharSetRemoveActor`) and re-syncs the actor
   records. */

void GameEventOp6ARemoveActor(GameEvent *self, u8 flag, s16 arg)

{
  CoreTask *task;
  
  task = CoreTaskFind(500);
  GameFieldPartyRemove(task,flag);
  GameEvent470SyncActorRecords((GameEvent470 *)self);
  return;
}

