// bdc 0x088f0c3c GameEventOp79AddActorRotY
#include "bdc.h"

/* Handler of event opcode 0x79 (`GameEvent470ExecCommand`): sets the target angle y to the
   current angle (`+0x32`) plus `arg` degrees. */

void GameEventOp79AddActorRotY(GameEvent470 *self, u8 flag, s16 arg)

{
  u32 idx = flag;
  GameEventActorRecord *rec;

  if (idx == 100) {
    idx = self->talkPartner;
  }
  rec = &self->actors[self->actorMap[idx]];
  rec->endRot[1] = rec->rot[1] + (u16)(s32)((float)arg * 65536.0f * 0.0027777778f);
}
