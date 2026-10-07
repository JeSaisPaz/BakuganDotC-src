// bdc 0x08818abc UiTextPrinterCreate
#include "bdc.h"

/* Allocates (low heap) and constructs a 0xf0-byte text printer (`UiTextPrinterCtor`) with the font
   texture list `0x08ab0158[font]` (`UiTextPrinterCtor`) and selects font `font`
   (`UiTextPrinterSetFont`). Returns the printer. Used by `GameDebugStageSelectCtor`,
   `UiConfirmDialogCtor`, `UiPauseCtor`. */

UiTextPrinter * UiTextPrinterCreate(s32 font)

{
  bool fromLow;
  UiTextPrinter *self;
  UiTextPrinter *self_00;
  
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  self = MemAlloc(0xf0,(char *)0x0,0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self_00 = (UiTextPrinter *)0x0;
  if (self != (UiTextPrinter *)0x0) {
    UiTextPrinterCtor(self,0,g_uiFontTextureLists[font]);
    self_00 = self;
  }
  UiTextPrinterSetFont(self_00,font);
  return self_00;
}

