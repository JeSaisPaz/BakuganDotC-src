// bdc 0x089fbbf4 IoDataGetLoadedSize
#include "bdc.h"

/* Returns the word `+0x5c` of a data request (`COData`) under its lock (the loaded size/data
   reported back to the sound manager). */

u32 IoDataGetLoadedSize(IoData *self)

{
  u32 size;
  
  CoreLockAcquire(self->lock);
  size = self->loadedSize;
  CoreLockRelease(self->lock);
  return size;
}

