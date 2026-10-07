// bdc 0x089cbc3c SysUtilHandlerCtor
#include "bdc.h"

/* Base constructor of a system-utility handler object: installs the base vtable `0x08af52bc` at
   `+0x10`, clears state (`+0x4`), id (`+0x8`) and `+0x0`, sets it disabled
   (`SysUtilHandlerSetEnabled`(0)) and, if the manager exists, registers it with
   `SysUtilRegister`. Returns `handler`. */

SysUtilHandler *SysUtilHandlerCtor(SysUtilHandler *self)

{
  u32 *cell;
  
  self->vtbl = g_sysUtilHandlerVtbl;
  self->baseWord = 0;
  self->state = 0;
  self->id = 0;
  SysUtilHandlerSetEnabled(self,'\0');
  if (SysUtilIsInit()) {
    cell = SysUtilGetCell();
    SysUtilRegister(cell,self);
  }
  return self;
}

