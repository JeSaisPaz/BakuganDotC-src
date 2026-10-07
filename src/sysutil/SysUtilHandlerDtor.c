// bdc 0x089cbca8 SysUtilHandlerDtor
#include "bdc.h"

/* Base destructor of a system-utility handler: restores the base vtable, unregisters the handler
   from the manager (`SysUtilUnregister`) if the manager exists and, when bit 0 of `flags` is set,
   frees the object. */

void SysUtilHandlerDtor(SysUtilHandler *self, u32 flags)

{
  u32 *cell;
  
  if (self != (SysUtilHandler *)0x0) {
    self->vtbl = g_sysUtilHandlerVtbl;
    if (SysUtilIsInit()) {
      cell = SysUtilGetCell();
      SysUtilUnregister(cell,self);
    }
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

