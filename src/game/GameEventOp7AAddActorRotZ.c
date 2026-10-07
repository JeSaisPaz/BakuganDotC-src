// bdc 0x088f0ce4 GameEventOp7AAddActorRotZ
#include "bdc.h"

/* Handler of event opcode 0x7a (`GameEvent470ExecCommand`): sets the target angle z to the
   current angle (`+0x34`) plus `arg` degrees. */

void GameEventOp7AAddActorRotZ(GameEvent470 *self, u8 flag, s16 arg)

{
  u32 idx = flag;

  if (idx == 100) {
    idx = self->talkPartner;
  }
  self->actors[self->actorMap[idx]].endRot[2] =
       self->actors[self->actorMap[idx]].rot[2] +
       (s16)(int)((float)arg * 65536.0f * 0.0027777778f);
  return;
}
