// bdc 0x089fdf58 IoDecodeGetOutput
#include "bdc.h"

/* Returns the output buffer (`+0x1068`) of a decode job (`CODecode`, 0x10b0 bytes) under its lock.
    */

void *IoDecodeGetOutput(IoDecodeJob *self)

{
  u8 *out;
  
  CoreLockAcquire(self->lock);
  out = self->output;
  CoreLockRelease(self->lock);
  return out;
}

