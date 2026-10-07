// bdc 0x08a2c73c GameEventActionGetTarget
#include "bdc.h"

/* Entry 6 (target index, `+0x34`) of the event action base vtable `0x08af6e20`, also used by the
   camera tween (`0x08af430c`): returns 0. */

u8 GameEventActionGetTarget(GameEventAction *self)

{
  return '\0';
}

