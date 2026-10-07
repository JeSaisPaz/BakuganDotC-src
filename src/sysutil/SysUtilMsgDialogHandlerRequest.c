// bdc 0x08a01288 SysUtilMsgDialogHandlerRequest
#include "bdc.h"

/* Request virtual (slot `+0x14`) of the PSP message-dialog handler (error dialog, vtable
   `g_msgDialogHandlerVtbl`; params block `0x2c8` bytes in `g_msgDialogParams`): fills the
   common dialog header (`SysUtilInitDialogCommon`, size 0x2c4), sets mode 0 (error) and the error
   value `errorValue` (`+0x38`), sets the busy word `+0x2c4` to 1 and enables the handler. Returns
   1. */

int SysUtilMsgDialogHandlerRequest(void **handler, u32 errorValue)

{
  SysUtilInitDialogCommon(handler,g_msgDialogParams,0x2c4);
  (g_msgDialogParams->params).mode = PSP_UTILITY_MSGDIALOG_MODE_ERROR;
  (g_msgDialogParams->params).errorValue = errorValue;
  g_msgDialogParams->busy = 1;
  SysUtilHandlerSetEnabled((SysUtilHandler *)handler,'\x01');
  return 1;
}

