// bdc 0x089f9bbc IoDiscIsWaitingForBuffer
#include "bdc.h"

/* Returns whether the `CODiscSimple` disc reader (`g_discSimple`) is in state 3 (streamed read
   waiting for a free buffer), read under its lock. */

bool IoDiscIsWaitingForBuffer(IoDiscSimple *self)

{
  int state;
  
  CoreLockAcquire(self->lock);
  state = self->state;
  CoreLockRelease(self->lock);
  return state == 3;
}

