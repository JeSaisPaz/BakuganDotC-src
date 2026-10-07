// bdc 0x0896fb40 UiOptionCreateHelpPrinter
#include "bdc.h"

/* Creates the help text printer of `UiOption`: zeroes the 100 bytes of help state
   from `helpPrinter` to `helpText`, allocates a 0xf0-byte `UiTextPrinterCtor` from the low heap
   (`helpPrinter`, NULL when the allocation fails), selects font 1, sets scale 0.8, wrap width 1000
   and width scale 0.5333, clears `helpText` and sets `helpAlpha` = 0, `helpAlphaShown` = 1. */

void UiOptionCreateHelpPrinter(UiOption *self)
{
  bool fromLow;
  UiTextPrinter *mem;
  UiTextPrinter *printer;

  memset(&self->helpPrinter, 0, 100);
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  mem = MemAlloc(sizeof(UiTextPrinter), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  printer = NULL;
  if (mem != NULL) {
    UiTextPrinterCtor(mem, 0, NULL);
    printer = mem;
  }
  self->helpPrinter = printer;
  UiTextPrinterSetFont(printer, 1);
  self->helpPrinter->scale = 0.8f;
  self->helpPrinter->wrapWidth = 1000.0f;
  self->helpPrinter->widthScale = 0.53333336f;
  strcpy(self->helpText, "");
  self->helpAlpha = 0.0f;
  self->helpAlphaShown = 1.0f;
}
