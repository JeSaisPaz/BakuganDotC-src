// bdc 0x088f0ae8 GameEventOp76SetActorRotZ
#include "bdc.h"

/* Handler of event opcode 0x76 (`GameEvent470ExecCommand`): sets the target angle z (`+0x3a`) of
   event actor `flag` to `arg` degrees. */

void GameEventOp76SetActorRotZ(GameEvent470 *self, u8 flag, s16 arg)

{
  u32 idx = flag;

  if (idx == 100) {
    idx = self->talkPartner;
  }
  self->actors[self->actorMap[idx]].endRot[2] = (s16)(int)((float)(int)arg * 65536.0f * 0.0027777778f);
}
