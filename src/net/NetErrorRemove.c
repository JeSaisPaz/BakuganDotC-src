// bdc 0x089d2494 NetErrorRemove
#include "bdc.h"

/* Under the manager lock, removes the first queued error record whose code equals `code`
   (`NetErrorListRemove`) and returns it to the pool or heap. */

void NetErrorRemove(void *mgr, s32 code)

{
  NetErrorMgr *m = mgr;
  s32 *data;

  CoreLockAcquire(m->lock);
  NetErrorListMerge(m->list);
  while ((data = NetErrorListNext(m->list)) != (s32 *)0x0) {
    if (*data != code) continue;
    NetErrorListRemove(m->list,data);
    if (g_netErrorMgr[1] == (MemPool *)0x0 || !MemPoolFree(g_netErrorMgr[1],data)) {
      MemLock();
      MemFree(data,(char *)0x0,0);
      MemUnlock();
    }
    break;
  }
  NetErrorListMerge(m->list);
  CoreLockRelease(m->lock);
  return;
}
