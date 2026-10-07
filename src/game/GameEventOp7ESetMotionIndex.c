// bdc 0x088f0f74 GameEventOp7ESetMotionIndex
#include "bdc.h"

/* Handler of event opcode 0x7e (`GameEvent470ExecCommand`): stores the motion index `index` in
   `+0x28e` for `GameEventOp80SetActorMotion`. */

void GameEventOp7ESetMotionIndex(GameEvent470 *self, s16 index, s16 arg)

{
  self->motionIndex = index;
  return;
}

