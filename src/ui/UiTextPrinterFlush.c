// bdc 0x08818a8c UiTextPrinterFlush
#include "bdc.h"

/* Text printer vtable slot 5: draws the printer's sprite layer (`GfxSpriteLayerDraw`, render
   packet passed through), then clears it (`GfxSpriteLayerClear`) and resets the glyph list
   `+0x84`, so text is re-printed every frame. */

void UiTextPrinterFlush(UiTextPrinter *self, void *packet)

{
  GfxSpriteLayerDraw(&self->layer,packet);
  GfxSpriteLayerClear(&self->layer);
  self->glyphs = (GfxSprite *)0x0;
  return;
}

