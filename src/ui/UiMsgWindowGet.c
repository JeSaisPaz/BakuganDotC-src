// bdc 0x08816a6c UiMsgWindowGet
#include "bdc.h"

/* Returns the message window singleton `g_uiMsgWindow` (may be NULL). */

void *UiMsgWindowGet(void)

{
  return g_uiMsgWindow;
}

