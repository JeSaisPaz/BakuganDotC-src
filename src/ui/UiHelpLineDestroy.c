// bdc 0x089a3cf0 UiHelpLineDestroy
#include "bdc.h"

/* Deletes the shared help-line printer (`g_helpLine` `printer`) through its virtual destructor
   (flag 3) and clears the pointer. Called by `UiPauseDtor` and `UiWorldMapDtor`. */

void UiHelpLineDestroy(void)

{
  const VtblEntry *e;
  
  if (g_helpLine.printer != (UiTextPrinter *)0x0) {
    e = &g_helpLine.printer->layer.vtbl[1];
    ((void (*)(void *, s32))e->fn)((u8 *)g_helpLine.printer + e->delta, 3);
    g_helpLine.printer = (UiTextPrinter *)0x0;
  }
  return;
}
