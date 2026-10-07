// bdc 0x088f0440 GameEventOp69AddActor
#include "bdc.h"

/* Handler of event opcode 0x69 (`GameEvent470ExecCommand`): spawns character-set entry `flag` in
   the field (`GameFieldPartyAdd` -> `GameFieldCharSetAddActor`) and re-syncs the actor records. */

void GameEventOp69AddActor(GameEvent *self, u8 flag, s16 arg)

{
  CoreTask *task;
  
  task = CoreTaskFind(500);
  GameFieldPartyAdd(task,flag);
  GameEvent470SyncActorRecords((GameEvent470 *)self);
  return;
}

