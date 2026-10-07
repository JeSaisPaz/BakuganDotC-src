// bdc 0x089d37a0 NetAdhocCreate
#include "bdc.h"

/* Creates the ad-hoc network manager `g_netAdhoc` (0x1fc zeroed bytes, low heap) and starts networking:
   sets the CPU clock to 222 MHz, creates the 8-entry packet queue `queue` (`NetPacketQueueCtor`), fills
   the adhocctl product struct `product` (type 0, code `"ULES01466"`), creates the COPSPNet connection
   object `conn` (`NetAdhocConnCtor`, 0x5c bytes) and a 0x20-entry list `list` (`NetAdhocListInit`),
   each from the low heap and left NULL when its allocation fails, then `NetErrorCreate`,
   `NetCharaMgrCreate`, `NetModeFlagSet` and starts thread 0x12 "MyThread-Network"
   (`BootStartThread`). */

void NetAdhocCreate(void)
{
  bool fromLow;
  NetAdhocManager *mgr;
  CoreBufQueue *queue;
  NetAdhocConn *conn;
  CoreList *list;

  CorePowerSetCpuClock(222);

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mgr = MemAlloc(sizeof(NetAdhocManager), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_netAdhoc = mgr;
  memset(mgr, 0, sizeof(NetAdhocManager));

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  queue = MemAlloc(sizeof(CoreBufQueue), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (queue != NULL) {
    NetPacketQueueCtor(queue, 8);
  }
  g_netAdhoc->queue = (NetPacketQueue *)queue;
  g_netAdhoc->product.unknown = 0;
  memcpy(g_netAdhoc->product.product, "ULES01466", 9);

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  conn = MemAlloc(sizeof(NetAdhocConn), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (conn != NULL) {
    NetAdhocConnCtor(conn);
  }
  g_netAdhoc->conn = conn;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  list = MemAlloc(sizeof(CoreList), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (list != NULL) {
    NetAdhocListInit(list, 0x20);
  }
  g_netAdhoc->list = list;

  NetErrorCreate();
  NetCharaMgrCreate();
  NetModeFlagSet();
  BootStartThread(0x12, NULL, 0);
}
