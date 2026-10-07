// bdc 0x089d20b4 NetErrorDestroy
#include "bdc.h"

/* Destroys the `CONetError` manager singleton `g_netErrorMgr`: destroys the record pool, runs
   `NetErrorMgrDtor``(mgr, 3)` and frees the holder. Counterpart of `NetErrorCreate`; called
   from the ad-hoc shutdown `NetAdhocDestroy`. */

void NetErrorDestroy(void)

{
  void *mgr;

  if (g_netErrorMgr != (void **)0x0) {
    if (g_netErrorMgr[1] != (MemPool *)0x0) {
      MemPoolDestroy(g_netErrorMgr[1],3);
      g_netErrorMgr[1] = (void *)0x0;
    }
    mgr = *g_netErrorMgr;
    if (mgr != (void *)0x0) {
      NetErrorMgrDtor(mgr,3);
      *g_netErrorMgr = (void *)0x0;
    }
    if (g_netErrorMgr != (void **)0x0) {
      MemLock();
      MemFree(g_netErrorMgr,(char *)0x0,0);
      MemUnlock();
      g_netErrorMgr = (void **)0x0;
    }
  }
  return;
}

