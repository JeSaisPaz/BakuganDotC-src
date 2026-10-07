// bdc 0x089a5bf0 UiHelpLineHide
#include "bdc.h"

/* Hides the shared help line by setting its alpha `0x08b0109c` to 0 (when the printer exists). */

void UiHelpLineHide(void)

{
  if (g_helpLinePrinter != (void *)0x0) {
    g_helpLineAlpha = 0.0f;
  }
  return;
}

