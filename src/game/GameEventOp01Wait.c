// bdc 0x088ebbe4 GameEventOp01Wait
#include "bdc.h"

/* Handler of event opcode 0x01 (`GameEventExecCommand`): unless in skip mode (`+0x273` bit 0),
   blocks the script for `arg` frames (wait counter `+0x260`, wait type `+0x269 = 1`, counted down
   by `GameEventCheckWaitDone`). */

void GameEventOp01Wait(GameEvent *self, s16 flag, s16 arg)

{
  
  if ((self->flags & 1) == 0) {
    self->waitFrames = flag;
    self->waitType = '\x01';
  }
  return;
}

