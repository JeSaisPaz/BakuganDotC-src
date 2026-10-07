// bdc 0x08978f3c UiCollectionSphereCreateHelpPrinter
#include "bdc.h"

/* Creates the description text printer of `UiCollectionSphere` (`+0xf10`,
   `UiTextPrinterCtor` with `"wd_font16"`, font 1, scale 0.9, wrap width 152), clears its buffer
   `+0xf34` and the help alpha fields `+0xf14..0xf1c`. */

void UiCollectionSphereCreateHelpPrinter(UiCollectionSphere *self)

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
    UiTextPrinterCtor(self_00,0,&g_uiCollectionSphereFontNames);
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

