// bdc 0x088f0b94 GameEventOp78AddActorRotX
#include "bdc.h"

/* Handler of event opcode 0x78 (`GameEvent470ExecCommand`): sets the target angle x of event
   actor `flag` to its current angle (`+0x30`) plus `arg` degrees. */

void GameEventOp78AddActorRotX(GameEvent470 *self, u8 flag, s16 arg)

{
  u32 idx = flag;
  GameEventActorRecord *rec;

  if (idx == 100) {
    idx = self->talkPartner;
  }
  rec = &self->actors[self->actorMap[idx]];
  rec->endRot[0] = rec->rot[0] + (u16)(s32)((float)arg * 65536.0f * 0.0027777778f);
}
