// bdc 0x089cc640 SysUtilSavedataHandlerDtor
#include "bdc.h"

/* Destructor of the savedata dialog handler (`SysUtilSavedataHandlerCtor`): reinstalls vtable
   `0x08af52f4`, frees the id block (`+0x5d4`) and the `SceUtilitySavedataParam` block `*0x08ac5908`
   (clearing the global), runs `SysUtilHandlerDtor` and frees the handler when `flags & 1`. */

void SysUtilSavedataHandlerDtor(SysUtilSavedataHandler *self, u32 flags)

{
  SceUtilitySavedataMsDataInfo *ptr;
  
  if (self != (SysUtilSavedataHandler *)0x0) {
    (self->base).vtbl = (VtblEntry *)&g_sysUtilSavedataVtbl;
    ptr = (g_savedataParams->params).msData;
    if (ptr != (SceUtilitySavedataMsDataInfo *)0x0) {
      MemLock();
      MemFree(ptr,(char *)0x0,0);
      MemUnlock();
      (g_savedataParams->params).msData = (SceUtilitySavedataMsDataInfo *)0x0;
    }
    if (g_savedataParams != (SysUtilSavedataBlock *)0x0) {
      MemLock();
      MemFree(g_savedataParams,(char *)0x0,0);
      MemUnlock();
      g_savedataParams = (SysUtilSavedataBlock *)0x0;
    }
    SysUtilHandlerDtor(&self->base,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

