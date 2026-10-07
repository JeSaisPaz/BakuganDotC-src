// bdc 0x089d3aac NetAdhocGetManager
#include "bdc.h"

/* Returns the COPSPNet connection object (`*g_netAdhoc`, 0x5c bytes, its `CoreLock` at `+0x30`)
   without a NULL test; callers check `NetAdhocHasManager` first. The NetPlay lobby handlers pass
   it to the adhoc step functions (`NetAdhocRequestConnect`, `NetAdhocBeginStop` ..). */

void *NetAdhocGetManager(void)

{
  return g_netAdhoc->conn;
}

