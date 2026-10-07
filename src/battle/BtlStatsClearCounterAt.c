// bdc 0x08854b70 BtlStatsClearCounterAt
#include "bdc.h"

/* Zeroes counter `index` (0..0x15) of a statistics record, or all 22 counters for `index` >= 0x16;
   negative indices do nothing. The code never writes v0: its only caller `BtlStatsCtor` has
   `stats` in v0 at the call and reads it back, so returning `stats` is the observed result. */
BtlStats *BtlStatsClearCounterAt(BtlStats *stats, int index)
{
    int i;

    if (index < 0) {
        return stats;
    }
    if (index < 22) {
        stats->counters[index] = 0;
        return stats;
    }
    for (i = 0; i < 22; i++) {
        stats->counters[i] = 0;
    }
    return stats;
}
