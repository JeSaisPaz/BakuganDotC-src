// bdc 0x088f3b10 GameEventPropTweenStop
#include "bdc.h"

/* Stop method (slot 3) of the prop tween action (vtable `0x08af437c`, 0xc bytes):
   `GameEventPropRecordFinish`. */

void GameEventPropTweenStop(GameEventPropTween *self, u8 apply)

{
  GameEventPropRecordFinish(self->rec,(uint)self->kind,apply);
  return;
}

