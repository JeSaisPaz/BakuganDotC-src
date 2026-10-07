// bdc 0x089fb62c IoDataRefPoolCreate
#include "bdc.h"

/* Creates the shared pool for data-request owner references (`g_ioDataRefPool`, a `MemPool` of
   0x50 entries of 0x28 bytes) unless it exists; the 0x14-byte pool header is allocated from the low
   end of the heap under MemLock. If the allocation fails the global stays NULL. Called by
   `IoDataMngCreate`. */

void IoDataRefPoolCreate(void)
{
  bool fromLow;
  MemPool *pool;

  if (g_ioDataRefPool != NULL) {
    return;
  }
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  pool = (MemPool *)MemAlloc(sizeof(MemPool), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (pool != NULL) {
    MemPoolInit(pool, 0x28, 0x50, true);
  }
  g_ioDataRefPool = pool;
}
