// bdc 0x08854e28 BtlStatsGetCounter
#include "bdc.h"

/* Returns the u32 counter `stats + 4 + index*4` of a unit's battle statistics block (unit
   `+0x170`); the getter twin of `BtlStatsAddCounter`. */

int BtlStatsGetCounter(BtlStats *stats, int index)
{
    return stats->counters[index];
}
