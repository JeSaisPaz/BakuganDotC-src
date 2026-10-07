// bdc 0x088f0a6c GameEventOp75SetActorRotY
#include "bdc.h"

/* Handler of event opcode 0x75 (`GameEvent470ExecCommand`): sets the target angle y (`+0x38`) of
   event actor `flag` to `arg` degrees. */

void GameEventOp75SetActorRotY(GameEvent470 *self, u8 flag, s16 arg)

{
  u32 idx;
  
  idx = (u32)flag;
  if (idx == 100) {
    idx = (u32)self->talkPartner;
  }
  self->actors[self->actorMap[idx]].endRot[1] =
       (s16)(int)((float)(int)arg * 65536.0f * 0.0027777778f);
  return;
}

