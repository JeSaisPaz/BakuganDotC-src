// bdc 0x089d1f68 NetErrorCreate
#include "bdc.h"

/* Creates the `CONetError` manager singleton `g_netErrorMgr` if it does not exist: allocates the
   8-byte holder `{mgr, pool}` from the low heap, then the `NetErrorMgr` (`NetErrorMgrCtor`)
   and a `MemPool` of 0x20 eight-byte error records (from low). A failed manager or pool
   allocation leaves its slot NULL; the holder allocation itself is not checked. Called from the
   ad-hoc start-up `NetAdhocCreate`. */

void NetErrorCreate(void)
{
  bool fromLow;
  void **holder;
  void *mgr;
  MemPool *pool;

  if (g_netErrorMgr != NULL) {
    return;
  }

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  holder = MemAlloc(8, NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_netErrorMgr = holder;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mgr = MemAlloc(sizeof(NetErrorMgr), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (mgr != NULL) {
    NetErrorMgrCtor(mgr);
  }
  g_netErrorMgr[0] = mgr;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  pool = MemAlloc(sizeof(MemPool), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (pool != NULL) {
    MemPoolInit(pool, 8, 0x20, true);
  }
  g_netErrorMgr[1] = pool;
}
