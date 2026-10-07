// bdc 0x089fbb00 IoDataHasFlags
#include "bdc.h"

/* Returns whether all bits of `mask` are set in the request flags (`+0x30`) of a data request
   (`COData`). */

bool IoDataHasFlags(IoData *self, u32 mask)

{
  u32 flags;

  CoreLockAcquire(self->lock);
  flags = self->flags;
  CoreLockRelease(self->lock);
  return (flags & mask) == mask;
}

