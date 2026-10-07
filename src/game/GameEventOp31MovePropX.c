// bdc 0x088ed724 GameEventOp31MovePropX
#include "bdc.h"

/* Handler of event opcode 0x31 (`GameEventExecCommand`): adds `arg` along axis 0 (current
   coordinate mode, relative to prop `flag`) to the prop's end position. */

void GameEventOp31MovePropX(GameEvent *self, u8 flag, s16 arg)

{
  GameEventPropRecord *rec;
  s32 delta[3];

  delta[0] = 0;
  delta[1] = 0;
  delta[2] = 0;
  GameEventApplyAxisOffset(self, 0, arg, delta, flag);
  rec = &self->props[flag];
  rec->endPos[0] = rec->endPos[0] + delta[0];
  rec->endPos[1] = rec->endPos[1] + delta[1];
  rec->endPos[2] = rec->endPos[2] + delta[2];
}
