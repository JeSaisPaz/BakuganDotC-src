// bdc 0x088f112c GameEventOp85RestartActor
#include "bdc.h"

/* Handler of event opcode 0x85 (`GameEvent470ExecCommand`): restarts the placed behaviour of the
   character `flag` (`GameFieldCharSetRestartActor`). */

void GameEventOp85RestartActor(GameEvent470 *self, u8 flag, s16 arg)

{
  u32 slot;
  
  slot = (u32)flag;
  if (slot == 100) {
    slot = (u32)self->talkPartner;
  }
  GameFieldCharSetRestartActor(g_gameFieldCharSet,self->actorMap[slot]);
  return;
}

