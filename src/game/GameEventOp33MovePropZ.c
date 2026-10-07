// bdc 0x088ed85c GameEventOp33MovePropZ
#include "bdc.h"

/* Handler of event opcode 0x33 (`GameEventExecCommand`): adds `arg` along axis 2 to the end
   position of prop slot `flag`. */

void GameEventOp33MovePropZ(GameEvent *self, u8 flag, s16 arg)
{
  s32 off[3];
  GameEventPropRecord *rec;

  off[0] = 0;
  off[1] = 0;
  off[2] = 0;
  GameEventApplyAxisOffset(self, 2, arg, off, flag);
  rec = &self->props[flag];
  rec->endPos[0] = rec->endPos[0] + off[0];
  rec->endPos[1] = rec->endPos[1] + off[1];
  rec->endPos[2] = rec->endPos[2] + off[2];
}
