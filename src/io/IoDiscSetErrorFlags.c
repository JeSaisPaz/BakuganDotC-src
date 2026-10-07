// bdc 0x089f9a98 IoDiscSetErrorFlags
#include "bdc.h"

/* Under the lock of the `CODiscSimple` disc reader (`g_discSimple`), sets the error-handling flag
   `+0x48` to `value` and clears the result flags `+0x49`/`+0x4a`. */

void IoDiscSetErrorFlags(IoDiscSimple *self, u8 value)

{
  CoreLockAcquire(self->lock);
  self->errorFlags = value;
  self->openDecided = '\0';
  self->openOk = '\0';
  CoreLockRelease(self->lock);
  return;
}

