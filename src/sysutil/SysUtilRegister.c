// bdc 0x089cc3ac SysUtilRegister
#include "bdc.h"

/* Adds `handler` to the manager's handler list with priority 0: `CorePrioListAdd`(list, handler,
   0), if the list exists. The first parameter (the cell from `SysUtilGetCell`) is ignored. */

void SysUtilRegister(u32 *cell, void *handler)

{
  if (g_sysUtilMng->handlers != (CorePrioList *)0x0) {
    CorePrioListAdd(g_sysUtilMng->handlers,handler,0);
  }
  return;
}

