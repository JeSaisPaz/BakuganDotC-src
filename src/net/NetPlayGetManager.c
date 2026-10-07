// bdc 0x0881b214 NetPlayGetManager
#include "bdc.h"

/* Returns the NetPlay manager object (`*g_netPlay`). The caller must have checked
   `NetPlayHasManager`; there is no NULL test. */

void *NetPlayGetManager(void)

{
  return *g_netPlay;
}

