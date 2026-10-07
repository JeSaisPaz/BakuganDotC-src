// bdc 0x088f09f0 GameEventOp74SetActorRotX
#include "bdc.h"

/* Handler of event opcode 0x74 (`GameEvent470ExecCommand`): sets the target angle x (`+0x36`) of
   event actor `flag` to `arg` degrees. */

void GameEventOp74SetActorRotX(GameEvent470 *self, u8 flag, s16 arg)

{
  u32 idx;
  
  idx = (u32)flag;
  if (idx == 100) {
    idx = (u32)self->talkPartner;
  }
  self->actors[self->actorMap[idx]].endRot[0] =
       (s16)(int)((float)(int)arg * 65536.0f * 0.0027777778f);
  return;
}

