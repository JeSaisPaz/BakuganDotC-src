// bdc 0x08854e50 BtlStatsClear
#include "bdc.h"

/* Zeroes the 22 counters of a unit's statistics block (see `BtlStatsGetCounter`). */
void BtlStatsClear(BtlStats *stats)
{
    int i;

    for (i = 0; i < 22; i++) {
        stats->counters[i] = 0;
    }
}
