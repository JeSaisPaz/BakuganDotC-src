// bdc 0x089d3a84 NetAdhocHasManager
#include "bdc.h"

/* Returns 1 when the ad-hoc network manager exists (`g_netAdhoc` non-NULL and its first word
   non-NULL), else 0. The guard that 21 callers use before `NetAdhocGetManager`; the NetPlay state
   handlers and `NetPlayLateUpdate` use it. */

bool NetAdhocHasManager(void)

{
  bool has;
  
  has = false;
  if ((g_netAdhoc != (NetAdhocManager *)0x0) && (g_netAdhoc->conn != (NetAdhocConn *)0x0)) {
    has = true;
  }
  return has;
}

