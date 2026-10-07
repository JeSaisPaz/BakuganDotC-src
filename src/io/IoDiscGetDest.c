// bdc 0x089f9e50 IoDiscGetDest
#include "bdc.h"

/* Returns the destination buffer (`+0x24`) of the `CODiscSimple` disc reader (`g_discSimple`)
   under its lock. */

void *IoDiscGetDest(IoDiscSimple *self)

{
  void *dest;
  
  CoreLockAcquire(self->lock);
  dest = self->dest;
  CoreLockRelease(self->lock);
  return dest;
}

