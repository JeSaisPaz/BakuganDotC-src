// bdc 0x089eb220 UiTextBoxClear
#include "bdc.h"

/* Clears the text box's printed text (`GfxSpriteLayerClear` on the printer, resets the printer's character
   count `+0x84` and the box's needs-clear byte `+0x8`). Does nothing without a printer. */

void UiTextBoxClear(UiTextBox *box)

{
  UiTextPrinter *self;
  
  self = box->printer;
  if (self != (UiTextPrinter *)0x0) {
    GfxSpriteLayerClear(&self->layer);
    self->glyphs = (GfxSprite *)0x0;
    box->drawn = '\0';
  }
  return;
}

