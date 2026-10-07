// bdc 0x089f9cc0 IoDiscIsBusy
#include "bdc.h"

/* Returns the busy flag (`+0xb`) of the `CODiscSimple` disc reader (`g_discSimple`) under its
   lock; used by the disc-access indicator (`UiLoadIconTaskUpdate`) and `NetPlayLateUpdate`. */

bool IoDiscIsBusy(IoDiscSimple *self)

{
  u8 busy;
  
  CoreLockAcquire(self->lock);
  busy = self->busy;
  CoreLockRelease(self->lock);
  return (bool)busy;
}

