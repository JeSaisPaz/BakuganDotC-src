// bdc 0x088f1a7c GameEventOpB2HideFieldIcons
#include "bdc.h"

/* Handler of event opcode 0xb2 (`GameEvent470ExecCommand`): hides the field task's three icon
   sprites (`GameFieldHideLocationSprites`). */

void GameEventOpB2HideFieldIcons(GameEvent *self, u8 flag, s16 arg)

{
  CoreTask *task;
  
  task = CoreTaskFind(500);
  GameFieldHideLocationSprites(task);
  return;
}

