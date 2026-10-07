// bdc 0x088f0708 GameEventOp70MoveActorX
#include "bdc.h"

/* Handler of event opcode 0x70 (`GameEvent470ExecCommand`): adds `arg` along axis 0 (current
   coordinate mode) to the end position of event actor `flag`. */

void GameEventOp70MoveActorX(GameEvent470 *self, u8 flag, s16 arg)
{
  s32 off[3];
  GameEventActorRecord *recs;
  u32 idx = flag;

  if (idx == 100) {
    idx = self->talkPartner;
  }
  off[0] = 0;
  off[1] = 0;
  off[2] = 0;
  GameEventApplyAxisOffset(&self->base, 0, arg, off, self->actorMap[idx]);
  recs = self->actors;
  recs[self->actorMap[idx]].endPos[0] = recs[self->actorMap[idx]].endPos[0] + off[0];
  recs[self->actorMap[idx]].endPos[1] = recs[self->actorMap[idx]].endPos[1] + off[1];
  recs[self->actorMap[idx]].endPos[2] = recs[self->actorMap[idx]].endPos[2] + off[2];
}
