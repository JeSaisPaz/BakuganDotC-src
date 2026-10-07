// bdc 0x089fe158 IoDecodeMngCtor
#include "bdc.h"

/* Constructor of the decode manager (`CODecodeMng`): a `CoreNodeOwner` list with vtable
   `0x08af590c` plus a `MemPool` of 8 decode jobs of 0x10b0 bytes at `+0x30`. */

IoDecodeMng *IoDecodeMngCtor(IoDecodeMng *self)

{
  bool fromLow;
  MemPool *pool;
  MemPool *result;
  
  CoreNodeOwnerCtor(&self->base);
  (self->base).vtable = g_ioDecodeMngVtbl;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  pool = MemAlloc(0x14,(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  result = (MemPool *)0x0;
  if (pool != (MemPool *)0x0) {
    MemPoolInit(pool,0x10b0,8,true);
    result = pool;
  }
  self->pool = result;
  return self;
}

