// bdc 0x08854e38 BtlStatsAddCounter
#include "bdc.h"

/* Adds `delta` to statistics counter `index`. */
void BtlStatsAddCounter(BtlStats *stats, s32 index, s32 delta)
{
    stats->counters[index] += delta;
}
