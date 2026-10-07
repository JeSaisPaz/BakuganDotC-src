// bdc 0x089fbcc0 IoDataSetUserData
#include "bdc.h"

/* Sets the user word `+0x44` of a data request (`COData`) under its lock. */

void IoDataSetUserData(IoData *self, u32 value)

{
  CoreLockAcquire(self->lock);
  self->bufferSize = value;
  CoreLockRelease(self->lock);
  return;
}

