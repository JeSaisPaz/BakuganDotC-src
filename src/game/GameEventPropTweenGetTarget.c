// bdc 0x08a2c774 GameEventPropTweenGetTarget
#include "bdc.h"

/* Entry 6 (target index, `+0x34`) of the prop tween action vtable `0x08af437c`: returns the byte
   `+0xb`. */

u8 GameEventPropTweenGetTarget(GameEventPropTween *self)

{
  return self->target;
}

