// bdc 0x088f04d0 GameEventOp6BSetActorVisible
#include "bdc.h"

/* Handler of event opcode 0x6b (`GameEvent470ExecCommand`): sets the visible byte `+0xb8` of the
   actor of entry `flag` to `arg` (`GameFieldCharSetSetActorVisible`). */

void GameEventOp6BSetActorVisible(GameEvent *self, u8 flag, s16 arg)

{
  GameFieldCharSetSetActorVisible(g_gameFieldCharSet,flag,(u8)arg);
  return;
}

