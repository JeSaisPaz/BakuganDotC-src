// bdc 0x08854dc4 BtlStatsDtor
#include "bdc.h"

/* Destructor of the statistics record (`g_btlStatsVtbl` slot 1, called by `BtlStatsDestroy`
   and `BtlStatsSystemShutdown`): restores the vtable and frees the record when `flags & 1`. */
void BtlStatsDtor(BtlStats *stats, u32 flags)
{
    if (stats == NULL) {
        return;
    }
    stats->vtbl = g_btlStatsVtbl;
    if (flags & 1) {
        MemLock();
        MemFree(stats, NULL, 0);
        MemUnlock();
    }
}
