// bdc 0x088f18bc GameEventOpACDisableGimmick9
#include "bdc.h"

/* Handler of event opcode 0xac (`GameEvent470ExecCommand`): forwards `arg` to the field task
   (`GameFieldActivateItemBox`), which disables (virtual slot 16) the active gimmick of kind 9
   whose record parameter equals `arg`. */

void GameEventOpACDisableGimmick9(GameEvent *self, s16 arg, s16 unused)

{
  CoreTask *task;
  
  task = CoreTaskFind(500);
  GameFieldActivateItemBox(task,arg);
  return;
}

