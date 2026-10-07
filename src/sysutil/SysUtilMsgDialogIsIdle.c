// bdc 0x08a01334 SysUtilMsgDialogIsIdle
#include "bdc.h"

/* Returns whether the busy word of the message-dialog parameter block `g_msgDialogParams` is 0
   (dialog finished). */

bool SysUtilMsgDialogIsIdle(void)

{
  return g_msgDialogParams->busy == 0;
}
