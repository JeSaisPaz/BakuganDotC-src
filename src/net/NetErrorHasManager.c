// bdc 0x089d214c NetErrorHasManager
#include "bdc.h"

/* Returns whether the `CONetError` manager exists (`g_netErrorMgr` and the manager it holds are
   non-NULL). */

bool NetErrorHasManager(void)

{
  if (g_netErrorMgr != (void **)0x0 && *g_netErrorMgr != (void *)0x0) {
    return true;
  }
  return false;
}
