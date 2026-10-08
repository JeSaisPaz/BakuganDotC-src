// bdc 0x089d3990 NetAdhocDestroy
#include "bdc.h"

/* Destroys the ad-hoc network manager `g_netAdhoc`: frees the `list` (`NetAdhocListDestroy`), the
   `queue` object (virtual delete), the connection object
   (`NetAdhocConnDtor`) and the manager block, then `NetCharaMgrDestroy`, `NetErrorDestroy`, restores
   the CPU clock to 333 MHz and ends thread 0x12 (`BootDeleteThread`). */

void NetAdhocDestroy(void)

{
  if (g_netAdhoc != NULL) {
    if (g_netAdhoc->list != NULL) {
      NetAdhocListDestroy(g_netAdhoc->list, 3);
      g_netAdhoc->list = NULL;
    }
    if (g_netAdhoc->queue != NULL) {
      const VtblEntry *dtor = &g_netAdhoc->queue->base.vtbl[1];
      ((void (*)(void *, s32))dtor->fn)((u8 *)g_netAdhoc->queue + dtor->delta, 3);
      g_netAdhoc->queue = NULL;
    }
    if (g_netAdhoc->conn != NULL) {
      NetAdhocConnDtor(g_netAdhoc->conn, 3);
      g_netAdhoc->conn = NULL;
    }
    if (g_netAdhoc != NULL) {
      MemLock();
      MemFree(g_netAdhoc, NULL, 0);
      MemUnlock();
      g_netAdhoc = NULL;
    }
  }
  NetCharaMgrDestroy();
  NetErrorDestroy();
  CorePowerSetCpuClock(0x14d);
  BootDeleteThread(0x12);
}
