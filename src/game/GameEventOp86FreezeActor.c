// bdc 0x088f1168 GameEventOp86FreezeActor
#include "bdc.h"

/* Handler of event opcode 0x86 (`GameEvent470ExecCommand`): calls virtual slot 16 (freeze) on the
   character `flag` (`GameFieldCharSetFreezeActor`). */

void GameEventOp86FreezeActor(GameEvent470 *self, u8 flag, s16 arg)

{
  u32 slot;
  
  slot = (u32)flag;
  if (slot == 100) {
    slot = (u32)self->talkPartner;
  }
  GameFieldCharSetFreezeActor(g_gameFieldCharSet,self->actorMap[slot]);
  return;
}

