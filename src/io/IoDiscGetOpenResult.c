// bdc 0x089f9ae0 IoDiscGetOpenResult
#include "bdc.h"

/* Returns the open result of the `CODiscSimple` disc reader (`g_discSimple`) under its lock: -1
   while undecided (`+0x49` clear), 1 when the file opened (`+0x4a`), 0 when it failed. */

int IoDiscGetOpenResult(IoDiscSimple *self)

{
  int result;
  
  result = -1;
  CoreLockAcquire(self->lock);
  if ((self->openDecided != '\0') && (result = 0, self->openOk != '\0')) {
    result = 1;
  }
  CoreLockRelease(self->lock);
  return result;
}

