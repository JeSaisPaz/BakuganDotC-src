// bdc 0x089da974 GmoFree
#include "bdc.h"

/* Free callback matching `GmoAlloc`: pointers inside the bump region are ignored (the region is
   released as a whole), everything else goes to `MemFree` under `MemLock`. */

void GmoFree(void *ptr)
{
    if (g_gmoBumpBase == NULL || ptr < g_gmoBumpBase ||
        (u8 *)g_gmoBumpBase + g_gmoBumpSize <= (u8 *)ptr) {
        MemLock();
        MemFree(ptr, NULL, 0);
        MemUnlock();
    }
}
