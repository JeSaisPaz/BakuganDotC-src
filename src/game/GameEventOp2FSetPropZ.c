// bdc 0x088ed528 GameEventOp2FSetPropZ
#include "bdc.h"

/* Handler of event opcode 0x2f (`GameEventExecCommand`): sets the end-position z (`record+0x14`)
   of prop slot `flag`. */

void GameEventOp2FSetPropZ(GameEvent *self, u8 flag, s16 arg)

{
  self->props[flag].endPos[2] = (s32)(((s64)arg << 12) / g_gameEventFixedScale);
}
