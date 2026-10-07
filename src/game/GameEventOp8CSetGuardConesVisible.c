// bdc 0x088f1210 GameEventOp8CSetGuardConesVisible
#include "bdc.h"

/* Handler of event opcode 0x8c (`GameEvent470ExecCommand`): passes `flag` to the field task's
   `GameFieldForwardPlayerFlag`, which sets the view-cone flag of all guards (`ActorSetGuardsViewConeVisible`).
    */

void GameEventOp8CSetGuardConesVisible(GameEvent *self, u8 flag, s16 arg)

{
  CoreTask *task;
  
  task = GameFieldFindTask();
  GameFieldForwardPlayerFlag(task,flag);
  return;
}

