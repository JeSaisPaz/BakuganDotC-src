// bdc 0x0896ffb4 UiOptionDrawHelp
#include "bdc.h"

/* Draws the help text printer of `UiOption` at depth 5000, first copying the help
   alpha `+0xbc0` into the glyph sprites when it changed (`+0xbc4`), and resets its outline colour
   to `g_colorWhite`. */

void UiOptionDrawHelp(UiOption *self)

{
  GfxSprite *glyph;
  UiTextPrinter *printer;
  int i;

  if (self->helpGlyphCount != 0.0f && self->helpAlphaShown != self->helpAlpha) {
    glyph = self->helpGlyphs;
    i = 0;
    if (0.0f < self->helpGlyphCount) {
      do {
        i = i + 1;
        glyph->alpha = self->helpAlpha;
        glyph = glyph->next;
      } while ((float)i < self->helpGlyphCount);
    }
    self->helpAlphaShown = self->helpAlpha;
  }
  printer = self->helpPrinter;
  printer->outlineColor[0] = g_colorWhite.x;
  printer->outlineColor[1] = g_colorWhite.y;
  printer->outlineColor[2] = g_colorWhite.z;
  printer->outlineColor[3] = g_colorWhite.w;
  printer = self->helpPrinter;
  GfxSpriteLayerDraw(&printer->layer, GfxNewRenderPacket(5000.0f));
}
