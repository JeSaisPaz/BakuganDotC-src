// bdc 0x088f13e8 GameEventOp97SetTransitionParam
#include "bdc.h"

/* Handler of event opcode 0x97 (`GameEvent470ExecCommand`): stores `arg` in the transition
   parameter `g_gameTransitionParam`. */

void GameEventOp97SetTransitionParam(GameEvent *self, s16 arg, s16 unused)

{
  
  g_gameTransitionParam = arg;
  return;
}

