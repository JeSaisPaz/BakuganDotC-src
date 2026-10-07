// bdc 0x089cc3e8 SysUtilUnregister
#include "bdc.h"

/* Removes `handler` from the manager's handler list with `CorePrioListRemove` (it is flagged and
   freed at the next merge). The first parameter is ignored. */

void SysUtilUnregister(u32 *cell, void *handler)

{
  if (g_sysUtilMng->handlers != (CorePrioList *)0x0) {
    CorePrioListRemove(g_sysUtilMng->handlers,handler);
  }
  return;
}

