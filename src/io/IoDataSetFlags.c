// bdc 0x089fba50 IoDataSetFlags
#include "bdc.h"

/* Overwrites the request flags (`+0x30`) of a data request (`COData`) under its lock and wakes the
   data thread (`BootWakeupThread(4)`). */

void IoDataSetFlags(IoData *self, u32 flags)

{
  CoreLockAcquire(self->lock);
  self->flags = flags;
  CoreLockRelease(self->lock);
  BootWakeupThread(4);
  return;
}

