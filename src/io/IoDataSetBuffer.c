// bdc 0x089fbc2c IoDataSetBuffer
#include "bdc.h"

/* Sets the data buffer (`+0x3c`) of a data request (`COData`) under its lock: 0 or 1 ask the
   request to allocate it (from the high/low heap end) and mark it owned (`+0x39` = 1); any other
   value is a caller buffer (`+0x39` = 0). */

void IoDataSetBuffer(IoData *self, void *buffer)
{
  CoreLockAcquire(self->lock);
  self->ownsBuffer = 1;
  if (buffer != NULL && (uintptr_t)buffer != 1) {
    self->ownsBuffer = 0;
  }
  self->buffer = buffer;
  CoreLockRelease(self->lock);
}
