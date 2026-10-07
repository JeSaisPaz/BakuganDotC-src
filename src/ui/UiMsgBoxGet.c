// bdc 0x089ebce8 UiMsgBoxGet
#include "bdc.h"

/* Returns the message box stored at `*g_uiMsgBoxHolder` (no NULL checks; guard with `UiMsgBoxExists`).
    */

void *UiMsgBoxGet(void)

{
  return *g_uiMsgBoxHolder;
}

