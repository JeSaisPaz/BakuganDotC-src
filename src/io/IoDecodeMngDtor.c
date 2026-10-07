// bdc 0x089fe204 IoDecodeMngDtor
#include "bdc.h"

/* Destructor of the decode manager (vtable `0x08af590c` slot 1): destroys the job pool, runs
   `CoreNodeOwnerDtor` and frees it when `flags & 1`. */

void IoDecodeMngDtor(IoDecodeMng *self, u32 flags)

{
  MemPool *pool;
  
  if (self != (IoDecodeMng *)0x0) {
    pool = self->pool;
    (self->base).vtable = g_ioDecodeMngVtbl;
    if (pool != (MemPool *)0x0) {
      MemPoolDestroy(pool,3);
      self->pool = (MemPool *)0x0;
    }
    CoreNodeOwnerDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

