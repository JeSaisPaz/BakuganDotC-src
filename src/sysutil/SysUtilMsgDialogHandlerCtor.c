// bdc 0x08a01138 SysUtilMsgDialogHandlerCtor
#include "bdc.h"

/* Constructor of the PSP message-dialog handler (error dialog, vtable `g_msgDialogHandlerVtbl`;
   params block `0x2c8` bytes in `g_msgDialogParams`): `SysUtilHandlerCtor`, vtable, allocates
   and zeroes the dialog parameter block in the low heap (`g_msgDialogParams`) and marks task
   20000 active (`CoreTaskSetExclusiveId`). Created by `SysUtilOpenDialog`. */

SysUtilHandler *SysUtilMsgDialogHandlerCtor(SysUtilHandler *self)

{
  bool fromLow;
  SysUtilMsgDialogBlock *s;
  
  SysUtilHandlerCtor(self);
  self->vtbl = (VtblEntry *)&g_msgDialogHandlerVtbl;
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  s = (SysUtilMsgDialogBlock *)MemAlloc(0x2c8,(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  g_msgDialogParams = s;
  memset(s,0,0x2c8);
  CoreTaskSetExclusiveId(20000);
  return self;
}

