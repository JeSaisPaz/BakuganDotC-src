// bdc 0x089d2184 NetErrorMgrCtor
#include "bdc.h"

/* Constructor of the `CONetError` manager `NetErrorMgr` (0x10 bytes): `lock` = a low-heap
   `CoreLock` named "CONetError" (kernel mutex), `list` = the pending-error list
   (`NetErrorListInit`, 0x14 bytes, no pool), `latch` cleared. A failed allocation leaves the
   field NULL. Returns `mgr`. */

void *NetErrorMgrCtor(void *mgr)
{
  NetErrorMgr *self = (NetErrorMgr *)mgr;
  bool fromLow;
  CoreLock *lock;
  CorePrioList *list;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  lock = MemAlloc(sizeof(CoreLock), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (lock != NULL) {
    CoreLockInit(lock, "CONetError", CORE_LOCK_MUTEX);
  }
  self->lock = lock;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  list = MemAlloc(sizeof(CorePrioList), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (list != NULL) {
    NetErrorListInit(list, 0);
  }
  self->list = list;
  self->latch = 0;
  return mgr;
}
