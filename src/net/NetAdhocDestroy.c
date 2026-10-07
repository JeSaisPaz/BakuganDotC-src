// bdc 0x089d3990 NetAdhocDestroy
#include "bdc.h"

/* Destroys the ad-hoc network manager `g_netAdhoc`: frees the `list` (`NetAdhocListDestroy`), the
   `queue` object (virtual delete), the connection object
   (`NetAdhocConnDtor`) and the manager block, then `NetCharaMgrDestroy`, `NetErrorDestroy`, restores
   the CPU clock to 333 MHz and ends thread 0x12 (`BootDeleteThread`). */

void NetAdhocDestroy(void)

{
  NetAdhocManager *mgr;

  mgr = g_netAdhoc;
  if (mgr != NULL) {
    if (mgr->list != NULL) {
      NetAdhocListDestroy(mgr->list, 3);
      mgr->list = NULL;
    }
    if (mgr->queue != NULL) {
      const VtblEntry *dtor = &mgr->queue->vtbl[1];
      ((void (*)(void *, s32))dtor->fn)((u8 *)mgr->queue + dtor->delta, 3);
      mgr->queue = NULL;
    }
    if (mgr->conn != NULL) {
      NetAdhocConnDtor(mgr->conn, 3);
      mgr->conn = NULL;
    }
    MemLock();
    MemFree(mgr, NULL, 0);
    MemUnlock();
    g_netAdhoc = NULL;
  }
  NetCharaMgrDestroy();
  NetErrorDestroy();
  CorePowerSetCpuClock(0x14d);
  BootDeleteThread(0x12);
}
