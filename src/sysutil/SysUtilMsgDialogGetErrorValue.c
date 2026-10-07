// bdc 0x08a01300 SysUtilMsgDialogGetErrorValue
#include "bdc.h"

/* Returns the error value of the message-dialog parameter block `g_msgDialogParams`, or 0 when
   no dialog exists. Used by `NetErrorUpdate`. */

u32 SysUtilMsgDialogGetErrorValue(void)

{
  u32 result;

  result = 0;
  if (g_msgDialogParams != NULL) {
    result = g_msgDialogParams->params.errorValue;
  }
  return result;
}
