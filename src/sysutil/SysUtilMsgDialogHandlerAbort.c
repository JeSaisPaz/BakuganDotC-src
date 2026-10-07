// bdc 0x08a0131c SysUtilMsgDialogHandlerAbort
#include "bdc.h"

/* Abort virtual (vtable `0x08af5a14` slot `+0x1c`) of the message-dialog handler: sets the busy
   word of `g_msgDialogParams` to 3 so `SysUtilMsgDialogHandlerService` calls
   `sceUtilityMsgDialogAbort`. Returns 1. */

int SysUtilMsgDialogHandlerAbort(SysUtilHandler *self)

{
  g_msgDialogParams->busy = 3;
  return 1;
}
