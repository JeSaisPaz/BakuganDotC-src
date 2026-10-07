// bdc 0x08854bac BtlStatsCtor
#include "bdc.h"

/* Constructor of the 0x170-byte statistics record: installs `g_btlStatsVtbl`, clears the owner,
   zeroes both quadwords of `vecPair` (stores of the VFPU bank constant C720 = (0,0,0,0)), clears all
   counters (`BtlStatsClearCounterAt` with index 0x17), sets `state`/`motion` to -1, zeroes the two
   27-word arrays `wordTableA`/`wordTableB` and sets `field164` to -1. */
void BtlStatsCtor(BtlStats *stats)
{
    s32 i;

    stats->vtbl = g_btlStatsVtbl;
    stats->owner = NULL;
    /* Two sv.q of the bank zero vector C720. */
    for (i = 0; i < 32; i++) {
        stats->vecPair[i] = 0;
    }
    /* The clear routine returns its argument (v0 still holds stats). */
    stats = BtlStatsClearCounterAt(stats, 0x17);
    stats->state = -1;
    stats->motion = -1;
    for (i = 0; i < 27; i++) {
        stats->wordTableB[i] = 0;
        stats->wordTableA[i] = 0;
    }
    stats->field164 = -1;
}
