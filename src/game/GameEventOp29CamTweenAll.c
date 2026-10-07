// bdc 0x088ed290 GameEventOp29CamTweenAll
#include "bdc.h"

/* Handler of event opcode 0x29 (`GameEventExecCommand`): queues three tweens of `flag` frames with
   kinds 2, 1, 2 (fov, target, fov; the eye is not included, probably a slip for 0, 1, 2). `arg` is unused. */

void GameEventOp29CamTweenAll(GameEvent *self, u8 flag, s16 arg) {
  u16 frames = (u16)flag;

  GameEventAddCamTween(self, frames, 2);
  GameEventAddCamTween(self, frames, 1);
  GameEventAddCamTween(self, frames, 2);
}
