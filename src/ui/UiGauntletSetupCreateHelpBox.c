// bdc 0x0893195c UiGauntletSetupCreateHelpBox
#include "bdc.h"

/* Creates the card-help text box of `UiGauntletSetup`: allocates a 0xf0-byte
   `UiTextPrinter` with the `"wd_font16"` font table (`0x08ac19e8`), font 1,
   scale 0.7, wrap width 116, line spacing ~0.43, stores it at `+0xed4`, empties the string buffer
   `+0xef8` and resets alpha state `+0xed8` = 0 / `+0xedc` = 1 / line count `+0xee0` = 0. */

void UiGauntletSetupCreateHelpBox(UiGauntletSetup *self)

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
    UiTextPrinterCtor(self_00,0,g_uiGauntletSetupFontNames);
    self_01 = self_00;
  }
  self->helpPrinter = self_01;
  UiTextPrinterSetFont(self_01,1);
  self->helpPrinter->scale = 0.7;
  self->helpPrinter->wrapWidth = 116.0;
  self->helpPrinter->widthScale = 0.43333334;
  strcpy(self->helpText,"");
  self->helpAlpha = 0.0;
  self->helpAppliedAlpha = 1.0;
  self->helpReveal = 0.0;
  return;
}

