// bdc 0x08971df0 UiOptionClearHelp
#include "bdc.h"

/* Clears the help text printer of `UiOption` (`GfxSpriteLayerClear`). */

void UiOptionClearHelp(UiOption *self)

{
  UiTextPrinter *self_00;
  
  self_00 = self->helpPrinter;
  if (self_00 != (UiTextPrinter *)0x0) {
    GfxSpriteLayerClear(&self_00->layer);
    self_00->glyphs = (GfxSprite *)0x0;
  }
  return;
}

