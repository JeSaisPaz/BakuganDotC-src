// bdc 0x088ef684 GameEventIsBusy
#include "bdc.h"

/* True when the event state `+0x264` is not 1 (idle). */

s32 GameEventIsBusy(GameEvent *self)

{
  if (self->state != '\x01') {
    return 1;
  }
  return 0;
}

