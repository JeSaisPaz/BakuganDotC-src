// bdc 0x088f1874 GameEventOpA9GimmickCommand
#include "bdc.h"

/* Handler of event opcode 0xa9 (`GameEvent470ExecCommand`): forwards `arg` to the field task
   (`GameFieldActivateGimmick`), which signals the active gimmick whose record parameter equals
   `arg`. */

void GameEventOpA9GimmickCommand(GameEvent *self, s16 arg, s16 unused)

{
  CoreTask *task;
  
  task = CoreTaskFind(500);
  GameFieldActivateGimmick(task,arg);
  return;
}

