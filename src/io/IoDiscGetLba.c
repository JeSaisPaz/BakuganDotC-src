// bdc 0x089f9ec0 IoDiscGetLba
#include "bdc.h"

/* Returns the file start sector (`+0xfc`, from `IoDiscSimpleStateGetLba`) of the `CODiscSimple`
   disc reader (`g_discSimple`), under its lock. */

u32 IoDiscGetLba(IoDiscSimple *self)

{
  u32 lba;
  
  CoreLockAcquire(self->lock);
  lba = self->lba;
  CoreLockRelease(self->lock);
  return lba;
}

