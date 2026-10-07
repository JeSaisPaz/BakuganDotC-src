// bdc 0x089f9e88 IoDiscGetSize
#include "bdc.h"

/* Returns the file size (`+0x28`) found by the last request of the `CODiscSimple` disc reader
   (`g_discSimple`), under its lock. */

u32 IoDiscGetSize(IoDiscSimple *self)

{
  u32 size;
  
  CoreLockAcquire(self->lock);
  size = self->size;
  CoreLockRelease(self->lock);
  return size;
}

