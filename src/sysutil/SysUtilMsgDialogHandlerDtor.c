// bdc 0x08a011d0 SysUtilMsgDialogHandlerDtor
#include "bdc.h"

/* Destructor of the PSP message-dialog handler (error dialog, vtable `g_msgDialogHandlerVtbl`;
   params block `0x2c8` bytes in `g_msgDialogParams`): frees the parameter block, clears the
   active task id, runs `SysUtilHandlerDtor` and frees the handler when `flags & 1`. */

void SysUtilMsgDialogHandlerDtor(SysUtilHandler *self, u32 flags)

{
  if (self != (SysUtilHandler *)0x0) {
    self->vtbl = (VtblEntry *)&g_msgDialogHandlerVtbl;
    if (g_msgDialogParams != (SysUtilMsgDialogBlock *)0x0) {
      MemLock();
      MemFree(g_msgDialogParams,(char *)0x0,0);
      MemUnlock();
      g_msgDialogParams = (SysUtilMsgDialogBlock *)0x0;
    }
    CoreTaskSetExclusiveId(0);
    SysUtilHandlerDtor(self,0);
    if ((flags & 1) != 0) {
      MemLock();
      MemFree(self,(char *)0x0,0);
      MemUnlock();
    }
  }
  return;
}

