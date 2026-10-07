// bdc 0x089d2174 NetErrorGetManager
#include "bdc.h"

/* Returns the `CONetError` manager held by `g_netErrorMgr` (no NULL check; callers test
   `NetErrorHasManager` first). */

void *NetErrorGetManager(void)

{
  return *g_netErrorMgr;
}

