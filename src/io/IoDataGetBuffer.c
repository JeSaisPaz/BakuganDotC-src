// bdc 0x089fbc88 IoDataGetBuffer
#include "bdc.h"

/* Returns the data buffer (`+0x3c`) of a data request (`COData`) under its lock;
   `IoLzsPackagePoll` reads the loaded file through it. */

void *IoDataGetBuffer(IoData *self)

{
  void *buffer;
  
  CoreLockAcquire(self->lock);
  buffer = self->buffer;
  CoreLockRelease(self->lock);
  return buffer;
}

