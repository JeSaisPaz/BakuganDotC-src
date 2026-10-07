// bdc 0x089d010c NetModeFlagSet
#include "bdc.h"

/* Sets the global flag `g_netModeFlag` to 1. Called by `NetAdhocCreate`,
   `NetBattleSyncTaskUpdate`, `BtlMainPhaseLoad` and `BtlMainPhaseBattle`. */
void NetModeFlagSet(void)
{
    g_netModeFlag = 1;
}
