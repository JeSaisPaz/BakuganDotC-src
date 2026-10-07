// bdc 0x089ebcc0 UiMsgBoxExists
#include "bdc.h"

/* Returns 1 when the message box holder `g_uiMsgBoxHolder` and the box it points to exist. */

bool UiMsgBoxExists(void)

{
  bool exists;

  exists = false;
  if ((g_uiMsgBoxHolder != (void **)0x0) && (*g_uiMsgBoxHolder != (void *)0x0)) {
    exists = true;
  }
  return exists;
}
