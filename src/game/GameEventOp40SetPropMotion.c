// bdc 0x088edd8c GameEventOp40SetPropMotion
#include "bdc.h"

/* Handler of event opcode 0x40 (`GameEventExecCommand`): selects the motion slot `+0x262` used by
   `GameEventOp42PropPlayMotion`. The command flag is unused. */

void GameEventOp40SetPropMotion(GameEvent *self, s16 motion, u8 flag)

{
  self->propMotion = motion;
}
