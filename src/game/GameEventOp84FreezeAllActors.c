// bdc 0x088f110c GameEventOp84FreezeAllActors
#include "bdc.h"

/* Handler of event opcode 0x84 (`GameEvent470ExecCommand`): calls virtual slot 16 (freeze) on
   every character (`GameFieldCharSetFreezeAll`). */

void GameEventOp84FreezeAllActors(GameEvent *self, u8 flag, s16 arg)

{
  GameFieldCharSetFreezeAll(g_gameFieldCharSet);
  return;
}

