// bdc 0x089d0128 NetSyncStateIs2
#include "bdc.h"

/* Returns whether the lock-step sync state word `g_netSyncState` (written by `NetCharaSyncFrames`)
   equals 2. Used by `NetPlayUpdate` and `NetBattleSyncTaskUpdate`; the meaning of state 2 is
   not established (positional name). */

int NetSyncStateIs2(void)

{
  return g_netSyncState == 2;
}

