// bdc 0x088f17b8 GameEventOpA4GimmickCommand0bc7
#include "bdc.h"

/* Handler of event opcode 0xa4 (`GameEvent470ExecCommand`): forwards `arg` to the field task
   (`GameFieldActivateSwitchGimmick`), which signals the active 0xbc7 gimmick whose record
   parameter (`GameGimmickGetRecordParam`) equals `arg`. */

void GameEventOpA4GimmickCommand0bc7(GameEvent *self, s16 arg, s16 unused)

{
  CoreTask *task;
  
  task = CoreTaskFind(500);
  GameFieldActivateSwitchGimmick(task,arg);
  return;
}

