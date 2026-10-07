// bdc 0x089d23c0 NetErrorPush
#include "bdc.h"

/* Posts a network error `code`: takes an 8-byte record from the `g_netErrorMgr` pool (or the low
   heap), stores `code`, and under the manager lock adds it to the error list with priority 0
   (`NetErrorListAdd`) and sets the 'new error' byte `+0xc`. */

void NetErrorPush(void *mgr, s32 code)

{
  NetErrorMgr *m = mgr;
  bool fromLow;
  s32 *data;
  
  data = MemPoolAlloc(g_netErrorMgr[1]);
  if (data == (s32 *)0x0) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    data = MemAlloc(8,(char *)0x0,0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
  }
  *data = code;
  CoreLockAcquire(m->lock);
  NetErrorListAdd(m->list,data,0);
  m->latch = 1;
  CoreLockRelease(m->lock);
  return;
}

