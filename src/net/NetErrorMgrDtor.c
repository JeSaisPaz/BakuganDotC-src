// bdc 0x089d2288 NetErrorMgrDtor
#include "bdc.h"

/* Destructor of the `CONetError` manager `NetErrorMgr`: does nothing for NULL. If the error
   list has entries, merges it and drains it, giving each record back to the `g_netErrorMgr`
   record pool (holder slot 1) or, when there is no pool or the pool refuses it, to the heap.
   Then destroys the list (`NetErrorListDestroy`) and the lock (`CoreLockDestroy`), clearing
   both fields, and frees `mgr` when bit 0 of `flags` is set. */

void NetErrorMgrDtor(void *mgr, u32 flags)
{
  NetErrorMgr *self = (NetErrorMgr *)mgr;
  CorePrioList *list;
  void *rec;

  if (self == NULL) {
    return;
  }
  if (NetErrorListHasEntries(self->list)) {
    NetErrorListMerge(self->list);
    while ((rec = NetErrorListNext(self->list)) != NULL) {
      MemPool *pool = (MemPool *)g_netErrorMgr[1];
      if (pool != NULL && MemPoolFree(pool, rec)) {
        continue;
      }
      MemLock();
      MemFree(rec, NULL, 0);
      MemUnlock();
    }
  }
  list = self->list;
  if (list != NULL) {
    NetErrorListDestroy(list, 3);
    self->list = NULL;
  }
  if (self->lock != NULL) {
    CoreLockDestroy(self->lock, 3);
    self->lock = NULL;
  }
  if ((flags & 1) != 0) {
    MemLock();
    MemFree(mgr, NULL, 0);
    MemUnlock();
  }
}
