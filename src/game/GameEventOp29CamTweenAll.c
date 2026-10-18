// bdc 0x088ed290 GameEventOp29CamTweenAll
#include "bdc.h"

/* Handler of event opcode 0x29 (`GameEventExecCommand`): queues three tweens of `frames` frames
   (the command's 16-bit argument, passed as `(u16)arg`) with kinds 2, 1, 2 (fov, target, fov; the eye
   is not included, probably a slip for 0, 1, 2). `unused` is ignored. */

void GameEventOp29CamTweenAll(GameEvent *self, u16 frames, s16 unused) {
  GameEventAddCamTween(self, frames, 2);
  GameEventAddCamTween(self, frames, 1);
  GameEventAddCamTween(self, frames, 2);
}
