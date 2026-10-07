// bdc 0x089fb8b8 IoDataSetPathCopy
#include "bdc.h"

/* Stores a heap copy of `path` in a data request (`COData`) (`+0x24`, freeing the previous copy;
   low heap end when `fromLow`) and points `+0x28` at it, under its lock. */

void IoDataSetPathCopy(IoData *self, char *path, bool fromLow)

{
  bool oldFromLow;
  size_t len;
  char *copy;
  
  CoreLockAcquire(self->lock);
  copy = self->pathCopy;
  if (copy != (char *)0x0) {
    MemLock();
    MemFree(copy,(char *)0x0,0);
    MemUnlock();
    self->pathCopy = (char *)0x0;
  }
  len = strlen(path);
  MemLock();
  oldFromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(fromLow);
  copy = MemAlloc(len + 1,(char *)0x0,0);
  MemSetAllocFromLow(oldFromLow);
  MemUnlock();
  self->pathCopy = copy;
  strcpy(copy,path);
  self->path = self->pathCopy;
  CoreLockRelease(self->lock);
  return;
}

