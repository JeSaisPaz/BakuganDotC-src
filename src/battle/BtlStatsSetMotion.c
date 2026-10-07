// bdc 0x08854e9c BtlStatsSetMotion
#include "bdc.h"

/* Records the last motion in the statistics record (called by `BtlBakuganPlayMotion`); the
   store only happens when the value changes. */
void BtlStatsSetMotion(BtlStats *stats, int motion)
{
    if (stats->motion != motion) {
        stats->motion = motion;
    }
}
