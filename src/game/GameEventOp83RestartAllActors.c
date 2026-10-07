// bdc 0x088f10ec GameEventOp83RestartAllActors
#include "bdc.h"

/* Handler of event opcode 0x83 (`GameEvent470ExecCommand`): restarts the placed behaviour of
   every character (`GameFieldCharSetRestartAll`). */

void GameEventOp83RestartAllActors(GameEvent *self, u8 flag, s16 arg)

{
  GameFieldCharSetRestartAll(g_gameFieldCharSet);
  return;
}

