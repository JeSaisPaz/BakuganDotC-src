// bdc 0x0890e828 UiConfirmDialogGetResult
#include "bdc.h"

/* Returns the confirm dialog's answer `g_uiConfirmDialogResult` (1 = yes, 0 = no answer yet;
   cleared by `UiConfirmDialogCtor`). */

s32 UiConfirmDialogGetResult(void)

{
  return g_uiConfirmDialogResult;
}

