// bdc 0x0890e804 UiConfirmDialogSetMessage
#include "bdc.h"

/* Copies `text` into the confirm dialog's shared message buffer `0x08ac0e88` (shown by
   `UiConfirmDialogDraw`). */

void UiConfirmDialogSetMessage(const char *text)

{
  strcpy(g_confirmDialogMessage,text);
  return;
}

