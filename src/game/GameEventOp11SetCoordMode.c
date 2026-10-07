// bdc 0x088ebe68 GameEventOp11SetCoordMode
#include "bdc.h"

/* Handler of event opcode 0x11 (`GameEventExecCommand`): sets the coordinate mode `+0x267` used
   by the move opcodes (`GameEventApplyAxisOffset`: 0 world, 1 camera-relative, 2 actor-relative,
   4 class-specific). */

void GameEventOp11SetCoordMode(GameEvent *self, u8 flag, s16 arg)

{
  self->coordMode = flag;
  return;
}

