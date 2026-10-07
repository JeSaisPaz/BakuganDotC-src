// bdc 0x089a38e8 UiHelpLineCreate
#include "bdc.h"

/* Creates the shared help-line text printer: clears the 100-byte help-line state block that starts
   at `g_helpLinePrinter` (through `g_helpLineText`), allocates a 0xf0-byte
   `UiTextPrinterCtor` printer from the low end of the heap (font 1, scale 0.8, wrap width 1000,
   width scale 0.5333), empties `g_helpLineText` and resets `g_helpLineAlpha` to 0 and
   `g_helpLineAppliedAlpha` to 1. */

void UiHelpLineCreate(void)

{
  bool fromLow;
  UiTextPrinter *mem;
  UiTextPrinter *printer;

  /* the original clears the whole 100-byte block 0x08b01098..0x08b010fb in one go */
  memset(&g_helpLinePrinter, 0, 100);
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
  g_helpLinePrinter = printer;
  UiTextPrinterSetFont(printer, 1);
  g_helpLinePrinter->scale = 0.8f;
  g_helpLinePrinter->wrapWidth = 1000.0f;
  g_helpLinePrinter->widthScale = 0.53333336f;
  strcpy(g_helpLineText, "");
  g_helpLineAlpha = 0.0f;
  g_helpLineAppliedAlpha = 1.0f;
}
