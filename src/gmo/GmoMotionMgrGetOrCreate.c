// bdc 0x089d95ac GmoMotionMgrGetOrCreate
#include "bdc.h"

/* Returns the motion manager singleton `g_gmoMotionMgr`, allocating it (0x14 bytes, low heap) and
   running `GmoMotionMgrCtor` on first use. If the allocation fails the global stays NULL and NULL is
   returned. */

void *GmoMotionMgrGetOrCreate(void)
{
  bool fromLow;
  void *mgr;

  if (g_gmoMotionMgr == NULL) {
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mgr = MemAlloc(sizeof(GmoMotionMgr), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (mgr != NULL) {
      GmoMotionMgrCtor(mgr);
    }
    g_gmoMotionMgr = mgr;
  }
  return g_gmoMotionMgr;
}
