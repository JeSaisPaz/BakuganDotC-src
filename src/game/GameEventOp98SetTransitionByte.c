// bdc 0x088f1400 GameEventOp98SetTransitionByte
#include "bdc.h"

/* Handler of event opcode 0x98 (`GameEvent470ExecCommand`): stores `flag` in `g_gameTransitionByte`. */

void GameEventOp98SetTransitionByte(GameEvent *self, u8 flag, s16 arg)

{
  g_gameTransitionByte = flag;
  return;
}

