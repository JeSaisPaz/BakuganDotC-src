// bdc 0x089f9b7c IoDiscSetFlagF4
#include "bdc.h"

/* Sets the byte flag `+0xf4` of the `CODiscSimple` disc reader (`g_discSimple`) under its lock.
    */

void IoDiscSetFlagF4(IoDiscSimple *self, u8 value)

{
  CoreLockAcquire(self->lock);
  self->abortFlag = value;
  CoreLockRelease(self->lock);
  return;
}

