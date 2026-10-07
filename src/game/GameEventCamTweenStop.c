// bdc 0x088f2f6c GameEventCamTweenStop
#include "bdc.h"

/* Stop method (slot 3) of the camera tween action (vtable `0x08af430c`, 0x10 bytes):
   `GameEventCamKeysFinish` for its kind. */

void GameEventCamTweenStop(GameEventCamTween *self, u8 apply)

{
  GameEventCamKeysFinish(self->keys,(uint)self->kind,apply);
  return;
}

