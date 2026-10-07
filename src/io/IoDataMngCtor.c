// bdc 0x089fce70 IoDataMngCtor
#include "bdc.h"

/* Constructor of the data manager (`CODataMng`, `g_ioDataMng`): clears the `idle` flag (`+0x10`),
   then allocates from the low heap end the `"CODataMng"` LwMutex `CoreLock` (`+0x8`), two
   `CoreNodeOwner` lists (`+0x0` active requests, `+0x4` owner references) and a `MemPool` of
   0x40 request nodes of 0x60 bytes (`+0xc`). Each member is NULL when its allocation fails.
   Returns `self`. */

IoDataMng *IoDataMngCtor(IoDataMng *self)
{
  bool prevFromLow;
  CoreLock *lock;
  CoreNodeOwner *list;
  MemPool *pool;

  self->idle = false;

  MemLock();
  prevFromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  lock = MemAlloc(sizeof(CoreLock), NULL, 0);
  MemSetAllocFromLow(prevFromLow);
  MemUnlock();
  if (lock != NULL) {
    CoreLockInit(lock, "CODataMng", CORE_LOCK_LWMUTEX);
  }
  self->lock = lock;

  MemLock();
  prevFromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  list = MemAlloc(sizeof(CoreNodeOwner), NULL, 0);
  MemSetAllocFromLow(prevFromLow);
  MemUnlock();
  if (list != NULL) {
    CoreNodeOwnerCtor(list);
  }
  self->requests = list;

  MemLock();
  prevFromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  list = MemAlloc(sizeof(CoreNodeOwner), NULL, 0);
  MemSetAllocFromLow(prevFromLow);
  MemUnlock();
  if (list != NULL) {
    CoreNodeOwnerCtor(list);
  }
  self->ownerRefs = list;

  self->pool = NULL;
  MemLock();
  prevFromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  pool = MemAlloc(sizeof(MemPool), NULL, 0);
  MemSetAllocFromLow(prevFromLow);
  MemUnlock();
  if (pool != NULL) {
    MemPoolInit(pool, sizeof(IoData), 0x40, true);
  }
  self->pool = pool;
  return self;
}
