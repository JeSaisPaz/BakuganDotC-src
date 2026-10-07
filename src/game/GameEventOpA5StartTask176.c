// bdc 0x088f17f0 GameEventOpA5StartTask176
#include "bdc.h"

/* Handler of event opcode 0xa5 (`GameEvent470ExecCommand`): unless skipping, creates task 0x176
   (`CoreTaskCreateDefault`, `arg` forwarded as its argument), waits on it (wait type 7) and puts
   the camera under event control. When skipping it only clears `blocking`. */

void GameEventOpA5StartTask176(GameEvent *self, s16 arg, s16 unused)

{
  if ((self->flags & 1) != 0) {
    self->blocking = '\0';
    return;
  }
  CoreTaskCreateDefault(0x176, (void *)(intptr_t)arg);
  self->waitType = 7;
  GameFieldCameraBeginHold((GameFieldCamera *)g_gfxActiveCamera);
  return;
}
