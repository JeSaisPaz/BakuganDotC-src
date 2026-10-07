// bdc 0x08854e70 BtlStatsSetState
#include "bdc.h"

/* Records the unit's state and sub-state in the statistics record (from `BtlBakuganUpdate`);
   returns true when either changed. */
bool BtlStatsSetState(BtlStats *stats, int state, int subState)
{
    if (stats->state != state || stats->subState != subState) {
        stats->state = state;
        stats->subState = subState;
        return true;
    }
    return false;
}
