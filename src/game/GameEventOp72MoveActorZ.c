// bdc 0x088f08d8 GameEventOp72MoveActorZ
#include "bdc.h"

/* Handler of event opcode 0x72 (`GameEvent470ExecCommand`): adds `arg` along axis 2 to the end
   position of event actor `flag`. */

void GameEventOp72MoveActorZ(GameEvent470 *self, u8 flag, s16 arg)
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
  GameEventApplyAxisOffset(&self->base, 2, arg, off, self->actorMap[idx]);
  recs = self->actors;
  recs[self->actorMap[idx]].endPos[0] = recs[self->actorMap[idx]].endPos[0] + off[0];
  recs[self->actorMap[idx]].endPos[1] = recs[self->actorMap[idx]].endPos[1] + off[1];
  recs[self->actorMap[idx]].endPos[2] = recs[self->actorMap[idx]].endPos[2] + off[2];
}
