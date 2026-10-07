// bdc 0x08a01354 SysUtilMsgDialogHandlerRefreshState
#include "bdc.h"

/* Stores `sceUtilityMsgDialogGetStatus()` in the dialog state word (`+4`) of the PSP message-dialog
   handler (error dialog, vtable `0x08af5a14`; params block `g_msgDialogParams`). */

void SysUtilMsgDialogHandlerRefreshState(SysUtilHandler *self)

{
  int status;

  status = sceUtilityMsgDialogGetStatus();
  self->state = status;
  return;
}
