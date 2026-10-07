// bdc 0x08931864 UiGauntletSetupCreateNameBox
#include "bdc.h"

/* Creates the card-name text box of `UiGauntletSetup`: allocates a 0xf0-byte
   `UiTextPrinter` from low memory (no font table), font 3, scale 0.5, wrap
   width 1000, line spacing 0.4, stores it at `+0xcb0`, empties the string buffer `+0xcd4` and
   resets alpha state `+0xcb4` = 0 / `+0xcb8` = 1 / line count `+0xcbc` = 0. */

void UiGauntletSetupCreateNameBox(UiGauntletSetup *self)

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
  self->namePrinter = self_01;
  UiTextPrinterSetFont(self_01,3);
  self->namePrinter->scale = 0.5;
  self->namePrinter->wrapWidth = 1000.0;
  self->namePrinter->widthScale = 0.40000004;
  strcpy(self->nameText,"");
  self->nameAlpha = 0.0;
  self->nameAppliedAlpha = 1.0;
  self->nameReveal = 0.0;
  return;
}

