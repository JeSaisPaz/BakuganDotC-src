// bdc 0x089fba98 IoDataAddFlags
#include "bdc.h"

/* ORs `mask` into the request flags (`+0x30`) of a data request (`COData`) under its lock; when new
   bits were added (other than the release bit 0x10) the done flag `+0x38` is cleared so the request
   runs again. Wakes the data thread. */

void IoDataAddFlags(IoData *self, u32 mask)

{
  CoreLockAcquire(self->lock);
  if (((self->flags & mask) != mask) && (self->flags = self->flags | mask, mask != 0x10)) {
    self->done = '\0';
  }
  CoreLockRelease(self->lock);
  BootWakeupThread(4);
  return;
}

