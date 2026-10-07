// bdc 0x08939598 UiUnlockResultCreateNameText
#include "bdc.h"

/* Creates the reward-name text printer of `UiUnlockResult`: a 0xf0-byte
   `UiTextPrinter` from low memory at `+0x638`, font 3, scale 0.7, wrap width
   1000, spacing 0.6; empties `+0x640` and clears its alpha state (`+0x778`, `+0x740`, `+0x750`). */

void UiUnlockResultCreateNameText(UiUnlockResult *self)

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
  self->printers[0] = self_01;
  UiTextPrinterSetFont(self_01,3);
  self->printers[0]->scale = 0.7;
  self->printers[0]->wrapWidth = 1000.0;
  self->printers[0]->widthScale = 0.6;
  strcpy(self->texts[0],"");
  self->textVisible[0] = '\0';
  self->textAlpha[0] = 0.0;
  self->textReveal[0] = 0.0;
  return;
}

