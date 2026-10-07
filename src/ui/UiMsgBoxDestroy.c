// bdc 0x089ebc4c UiMsgBoxDestroy
#include "bdc.h"

/* Deletes the message box singleton (`*0x08ac5d90`, `UiMsgBoxDtor`) and frees its holder. */

void UiMsgBoxDestroy(void)
{
  void **holder = g_uiMsgBoxHolder;

  if (holder != NULL) {
    if (*holder != NULL) {
      UiMsgBoxDtor((UiMsgBox *)*holder, 3);
      holder = g_uiMsgBoxHolder;
      *holder = NULL;
    }
    if (holder != NULL) {
      MemLock();
      MemFree(g_uiMsgBoxHolder, NULL, 0);
      MemUnlock();
      g_uiMsgBoxHolder = NULL;
    }
  }
}
