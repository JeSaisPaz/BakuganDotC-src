// bdc 0x089d3710 NetRefreshMallocStat
#include "bdc.h"

/* Refreshes the net-library heap statistics at `g_netMallocStat` (`SceNetMallocStat`: pool, maximum,
   free) with `sceNetGetMallocStat`; called after a successful adhocctl connect
   (`NetAdhocctlConnectStep`). */

void NetRefreshMallocStat(void)

{
  sceNetGetMallocStat(&g_netMallocStat);
  return;
}

