// bdc 0x0896cfec UiCardEquipCreateNamePrinter
#include "bdc.h"

/* Creates the card-name text printer of `UiCardEquip` (`+0x2a84`, 0xf0 bytes,
   `UiTextPrinterCtor` with `"wd_font16"`, scale 0.8, depth 1000) and clears its text buffer
   `+0x2a8c`. */

void UiCardEquipCreateNamePrinter(UiCardEquip *self)

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
    UiTextPrinterCtor(self_00,0,&g_uiCardEquipFontNames);
    self_01 = self_00;
  }
  self->namePrinter = self_01;
  UiTextPrinterSetFont(self_01,0);
  self->namePrinter->scale = 0.79999995f;
  self->namePrinter->wrapWidth = 1000.0f;
  self->namePrinter->widthScale = 0.35f;
  strcpy(self->nameText,"");
  self->nameDirty = '\0';
  self->nameAlpha = 0.0f;
  self->nameGlyphCount = 0.0f;
  return;
}

