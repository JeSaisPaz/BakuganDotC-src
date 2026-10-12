// bdc 0x089a5bf0 UiHelpLineHide
#include "bdc.h"

/* Hides the shared help line by setting its alpha (`g_helpLine` `alpha`) to 0 (when the printer
   exists). */

void UiHelpLineHide(void)

{
  if (g_helpLine.printer != (void *)0x0) {
    g_helpLine.alpha = 0.0f;
  }
  return;
}
