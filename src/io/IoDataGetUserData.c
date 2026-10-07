// bdc 0x089fbd00 IoDataGetUserData
#include "bdc.h"

/* Returns the user word `+0x44` of a data request (`COData`) under its lock. */

u32 IoDataGetUserData(IoData *self)

{
  u32 value;

  CoreLockAcquire(self->lock);
  value = self->bufferSize;
  CoreLockRelease(self->lock);
  return value;
}
