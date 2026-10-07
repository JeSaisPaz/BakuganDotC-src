// bdc 0x08a2c764 GameEventActorTweenGetTarget
#include "bdc.h"

/* Entry 6 (target index, `+0x34`) of the event-actor tween vtable `0x08af4344`: returns the byte
   `+0xb`. */

u8 GameEventActorTweenGetTarget(GameEventActorTween *self)

{
  return self->target;
}

