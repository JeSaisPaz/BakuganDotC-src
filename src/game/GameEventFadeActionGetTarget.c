// bdc 0x08a2c74c GameEventFadeActionGetTarget
#include "bdc.h"

/* Entry 6 (target index, `+0x34`) of the screen-fade action vtable `0x08af42d4`: returns the byte
   `+0xa`. */

u8 GameEventFadeActionGetTarget(GameEventFadeAction *self)

{
  return self->target;
}

