// bdc 0x089cbf24 SysUtilDestroy
#include "bdc.h"

/* Destroys the system-utility manager: deletes the selection cell, then — if any handlers are
   registered — repeatedly takes the next entry of the handler list (`CorePrioListNext`) and
   runs its virtual destructor (slot `+0xc`, argument 3), destroys the list
   (`CorePrioListDestroy`) and finally frees the manager block and clears `g_sysUtilMng`. */

void SysUtilDestroy(void)

{
  void *handler;
  SysUtilMng *mng;
  CorePrioList *list;
  
  if (g_sysUtilMng->cell == (u32 *)0x0) {
    list = g_sysUtilMng->handlers;
  }
  else {
    SysUtilCellDelete(g_sysUtilMng->cell,3);
    mng = g_sysUtilMng;
    g_sysUtilMng->cell = (u32 *)0x0;
    list = mng->handlers;
  }
  if ((list != (CorePrioList *)0x0) &&
     CorePrioListHasEntries(list)) {
    while (handler = CorePrioListNext(g_sysUtilMng->handlers), handler != (void *)0x0) {
      SysUtilHandler *h = (SysUtilHandler *)handler;
      const VtblEntry *e = h->vtbl + 1;
      ((void (*)(void *, u32))e->fn)((char *)h + e->delta, 3);
    }
  }
  mng = g_sysUtilMng;
  if (g_sysUtilMng->handlers != (CorePrioList *)0x0) {
    CorePrioListDestroy(g_sysUtilMng->handlers,3);
    mng = g_sysUtilMng;
    g_sysUtilMng->handlers = (CorePrioList *)0x0;
  }
  if (mng != (SysUtilMng *)0x0) {
    MemLock();
    MemFree(g_sysUtilMng,(char *)0x0,0);
    MemUnlock();
    g_sysUtilMng = (SysUtilMng *)0x0;
  }
  return;
}

