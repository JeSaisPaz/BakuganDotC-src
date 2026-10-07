// bdc 0x089fa040 IoDiscIsSuspended
#include "bdc.h"

/* Returns 1 when the `CODiscSimple` disc reader (`g_discSimple`) is in state 9 (suspended after a
   power event or error), except for `mode == 1` while the power-suspend flag `+0xe1` is still set.
    */

int IoDiscIsSuspended(IoDiscSimple *self, int mode)

{
  int ret;
  
  ret = 0;
  CoreLockAcquire(self->lock);
  if (((self->state == 9) && (ret = 1, mode == 1)) && (self->powerSuspend != '\0')) {
    ret = 0;
  }
  CoreLockRelease(self->lock);
  return ret;
}

