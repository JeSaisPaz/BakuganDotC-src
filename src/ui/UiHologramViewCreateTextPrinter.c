// bdc 0x08928eec UiHologramViewCreateTextPrinter
#include "bdc.h"

/* Creates the 0xf0-byte text printer `+0x4c0` of the hologram detail view (`UiHologramViewCtor`,
   task 392; view kind `+0x485`) with the `wd_font16` font table `0x08ac1330`
   (`UiTextPrinterCtor`, `UiTextPrinterSetFont`) and sets its scale, wrap width 256 and colours.
    */

void UiHologramViewCreateTextPrinter(UiHologramView *self)

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
    UiTextPrinterCtor(self_00,0,g_uiHologramViewFontNames);
    self_01 = self_00;
  }
  self->printer = self_01;
  UiTextPrinterSetFont(self_01,0);
  self->printer->scale = 1.0;
  self->printer->wrapWidth = 256.0;
  self->printer->widthScale = 1.0;
  strcpy(self->text,"");
  self->shownAlpha = 1.0f;
  return;
}

