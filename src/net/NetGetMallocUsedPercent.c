// bdc 0x089d3730 NetGetMallocUsedPercent
#include "bdc.h"

/* Returns the used share of the net-library heap in percent, `100 - free*100/pool` from
   `sceNetGetMallocStat` into `g_netMallocStat`; returns 100 when the ad-hoc manager is missing or the
   call fails. */

s32 NetGetMallocUsedPercent(void)

{
  s32 percent;

  percent = 100;
  if (NetAdhocHasManager()) {
    if (sceNetGetMallocStat(&g_netMallocStat) == 0) {
      percent = 100 - (g_netMallocStat.free * 100) / g_netMallocStat.pool;
    }
  }
  return percent;
}
