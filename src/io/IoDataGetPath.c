// bdc 0x089fb9a0 IoDataGetPath
#include "bdc.h"

/* Returns the path (`+0x28`) of a data request (`COData`) under its lock. */

char *IoDataGetPath(IoData *self)

{
  char *path;
  
  CoreLockAcquire(self->lock);
  path = self->path;
  CoreLockRelease(self->lock);
  return path;
}

