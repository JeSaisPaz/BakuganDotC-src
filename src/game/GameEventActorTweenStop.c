// bdc 0x088f34fc GameEventActorTweenStop
#include "bdc.h"

/* Stop method (slot 3) of the event-actor tween action (vtable `0x08af4344`, 0xc bytes):
   `GameEventActorRecordFinish`. */

void GameEventActorTweenStop(GameEventActorTween *self, u8 apply)

{
  GameEventActorRecordFinish(self->rec,(uint)self->kind,apply);
  return;
}

