// bdc 0x089eb11c UiTextBoxPrint
#include "bdc.h"

/* Prints `text` at screen position (`x`, `y`) into the text box: clears the printer first if the
   box was drawn since the last print (byte `+0x8`), then calls the printer's virtual print method
   (vtable `+0x74`, slot 2) with the position, the two flag bytes and depth 0. Empty strings only
   clear. */

void UiTextBoxPrint(UiTextBox *box, s32 x, s32 y, char *text, u8 flagA, u8 flagB)

{
  UiTextPrinter *self;
  const VtblEntry *vt;

  self = box->printer;
  if (self != (UiTextPrinter *)0x0) {
    if (box->drawn != '\0') {
      GfxSpriteLayerClear(&self->layer);
      self->glyphs = (GfxSprite *)0x0;
      box->drawn = '\0';
    }
    if (*text != '\0') {
      self = box->printer;
      vt = self->layer.vtbl + 2;
      ((void (*)(float, float, float, void *, char *, u8, u8, u8))vt->fn)(
          (float)x, (float)y, 0.0f, (char *)self + vt->delta, text, flagA, 0, flagB);
    }
  }
}
