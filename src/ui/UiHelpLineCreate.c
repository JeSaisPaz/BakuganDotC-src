// bdc 0x089a38e8 UiHelpLineCreate
#include "bdc.h"

/* Creates the shared help-line text printer: clears the 100-byte help-line state `g_helpLine`
   (`UiHelpLine`), allocates a 0xf0-byte `UiTextPrinterCtor` printer from the low end of the
   heap (font 1, scale 0.8, wrap width 1000, width scale 0.5333), empties the text and resets the
   alpha to 0 and the applied alpha to 1. */

void UiHelpLineCreate(void)

{
  bool fromLow;
  UiTextPrinter *mem;
  UiTextPrinter *printer;

  memset(&g_helpLine, 0, sizeof(g_helpLine));
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(UiTextPrinter), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  printer = NULL;
  if (mem != NULL) {
    UiTextPrinterCtor(mem, NULL, NULL);
    printer = mem;
  }
  g_helpLine.printer = printer;
  UiTextPrinterSetFont(printer, 1);
  g_helpLine.printer->scale = 0.8f;
  g_helpLine.printer->wrapWidth = 1000.0f;
  g_helpLine.printer->widthScale = 0.53333336f;
  strcpy(g_helpLine.text, "");
  g_helpLine.alpha = 0.0f;
  g_helpLine.appliedAlpha = 1.0f;
}
