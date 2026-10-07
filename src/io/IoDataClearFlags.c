// bdc 0x089fbb5c IoDataClearFlags
#include "bdc.h"

/* Clears the bits of `mask` in the request flags (`+0x30`) of a data request (`COData`) under its
   lock. */

void IoDataClearFlags(IoData *self, u32 mask)

{
  CoreLockAcquire(self->lock);
  self->flags = self->flags & (mask ^ 0xffffffff);
  CoreLockRelease(self->lock);
  return;
}

