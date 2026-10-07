// bdc 0x0898afb0 UiCollectionFigureCreateHelpPrinter
#include "bdc.h"

/* Creates the description text printer of `UiCollectionFigure` (`+0xea8`,
   `UiTextPrinterCtor` with `"wd_font16"`, font 1, scale 0.9, wrap width 152), clears its buffer
   `+0xecc` and the help alpha fields `+0xeac..0xeb4`. */

void UiCollectionFigureCreateHelpPrinter(UiCollectionFigure *self)

{
  bool fromLow;
  UiTextPrinter *self_00;
  UiTextPrinter *self_01;
  
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  self_00 = MemAlloc(sizeof(UiTextPrinter),(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self_01 = (UiTextPrinter *)0x0;
  if (self_00 != (UiTextPrinter *)0x0) {
    UiTextPrinterCtor(self_00,0,g_collectionFigureFontNames);
    self_01 = self_00;
  }
  self->helpPrinter = self_01;
  UiTextPrinterSetFont(self_01,1);
  self->helpPrinter->scale = 0.9;
  self->helpPrinter->wrapWidth = 152.0;
  self->helpPrinter->widthScale = 0.56666666;
  strcpy(self->helpText,"");
  self->helpAlpha = 0.0;
  self->fadeFrom = 1.0;
  self->textLen = 0.0;
  return;
}

