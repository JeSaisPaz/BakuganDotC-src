// bdc 0x08816a10 UiMsgWindowDestroy
#include "bdc.h"

/* Destroys the message window singleton `g_uiMsgWindow` (`UiMsgWindowDtor(win, 3)`, destruct and
   free) and clears the pointer. Counterpart of `UiMsgWindowEnsure`. */

void UiMsgWindowDestroy(void)

{
  if (g_uiMsgWindow != (void *)0x0) {
    UiMsgWindowDtor(g_uiMsgWindow,3);
    g_uiMsgWindow = (void *)0x0;
  }
  return;
}

