// bdc 0x088ed7c0 GameEventOp32MovePropY
#include "bdc.h"

/* Handler of event opcode 0x32 (`GameEventExecCommand`): adds `arg` along axis 1 to the end
   position of prop slot `flag`. */

void GameEventOp32MovePropY(GameEvent *self, u8 flag, s16 arg)
{
  s32 off[3];
  GameEventPropRecord *rec;

  off[0] = 0;
  off[1] = 0;
  off[2] = 0;
  GameEventApplyAxisOffset(self, 1, arg, off, flag);
  rec = &self->props[flag];
  rec->endPos[0] = rec->endPos[0] + off[0];
  rec->endPos[1] = rec->endPos[1] + off[1];
  rec->endPos[2] = rec->endPos[2] + off[2];
}
