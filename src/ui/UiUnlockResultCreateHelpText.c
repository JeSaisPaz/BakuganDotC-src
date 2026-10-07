// bdc 0x0893968c UiUnlockResultCreateHelpText
#include "bdc.h"

/* Creates the help text printer of `UiUnlockResult`: a 0xf0-byte
   `UiTextPrinter` at `+0x63c`, font 0, scale 1, wrap width 296, spacing 1;
   empties `+0x6c0` and clears its alpha state (`+0x779`, `+0x744`, `+0x754`). */

void UiUnlockResultCreateHelpText(UiUnlockResult *self)

{
  bool fromLow;
  UiTextPrinter *self_00;
  UiTextPrinter *self_01;
  
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  self_00 = MemAlloc(0xf0,(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self_01 = (UiTextPrinter *)0x0;
  if (self_00 != (UiTextPrinter *)0x0) {
    UiTextPrinterCtor(self_00,0,(char **)0x0);
    self_01 = self_00;
  }
  self->printers[1] = self_01;
  UiTextPrinterSetFont(self_01,0);
  self->printers[1]->scale = 1.0;
  self->printers[1]->wrapWidth = 296.0;
  self->printers[1]->widthScale = 1.0;
  strcpy(self->texts[1],"");
  self->textVisible[1] = '\0';
  self->textAlpha[1] = 0.0;
  self->textReveal[1] = 0.0;
  return;
}

