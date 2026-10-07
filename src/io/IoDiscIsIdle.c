// bdc 0x089f9da4 IoDiscIsIdle
#include "bdc.h"

/* Returns the idle flag (`+0xc`) of the `CODiscSimple` disc reader (`g_discSimple`); when not
   idle and the disc thread (slot 2) is sleeping, wakes it. */

bool IoDiscIsIdle(IoDiscSimple *self)

{
  u8 idle;
  
  CoreLockAcquire(self->lock);
  idle = self->idle;
  CoreLockRelease(self->lock);
  if ((idle == '\0') && BootIsThreadSleeping(2)) {
    BootWakeupThread(2);
  }
  return (bool)idle;
}

