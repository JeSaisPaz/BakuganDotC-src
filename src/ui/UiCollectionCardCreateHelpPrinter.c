// bdc 0x08982378 UiCollectionCardCreateHelpPrinter
#include "bdc.h"

/* Creates the card description printer of `UiCollectionCard` (`+0xcd8`,
   `UiTextPrinterCtor` with `"wd_font16"`, font 1, scale 0.9, wrap width 152), clears its buffer
   `+0xcfc` and the help alpha fields. */

void UiCollectionCardCreateHelpPrinter(UiCollectionCard *self)

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
    UiTextPrinterCtor(self_00,0,&g_uiCollectionCardFontNames);
    self_01 = self_00;
  }
  self->helpPrinter = self_01;
  UiTextPrinterSetFont(self_01,1);
  self->helpPrinter->scale = 0.9;
  self->helpPrinter->wrapWidth = 152.0;
  self->helpPrinter->widthScale = 0.56666666;
  strcpy(self->helpText,"");
  self->helpAlpha = 0.0;
  self->helpAlphaApplied = 1.0;
  self->helpHeight = 0.0;
  return;
}

