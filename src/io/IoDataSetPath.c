// bdc 0x089fb878 IoDataSetPath
#include "bdc.h"

/* Sets the path pointer (`+0x28`) of a data request (`COData`) under its lock, without copying. */

void IoDataSetPath(IoData *self, char *path)

{
  CoreLockAcquire(self->lock);
  self->path = path;
  CoreLockRelease(self->lock);
  return;
}

