// bdc 0x08957b14 UiEquipDrawHelpText
#include "bdc.h"

/* Draws the help text printer `helpPrinter` of `UiEquip`: when dirty (`helpDirty`) and
   `helpGlyphCount` is non-zero, applies `helpAlpha` to the glyph list `helpGlyphs`; then copies
   `g_colorWhite` into the printer's outline colour (one VFPU quad copy) and submits it
   (`GfxNewRenderPacket``(4000)`, `GfxSpriteLayerDraw`). */

void UiEquipDrawHelpText(UiEquip *self)

{
  UiTextPrinter *printer;
  GfxSprite *glyph;
  int i;

  if (self->helpPrinter != NULL) {
    if (self->helpGlyphCount != 0.0f && self->helpDirty != 0) {
      glyph = self->helpGlyphs;
      i = 0;
      if (0.0f < self->helpGlyphCount) {
        do {
          i++;
          glyph->alpha = self->helpAlpha;
          glyph = glyph->next;
        } while ((float)i < self->helpGlyphCount);
      }
      self->helpDirty = 0;
    }
    printer = self->helpPrinter;
    printer->outlineColor[0] = g_colorWhite.x;
    printer->outlineColor[1] = g_colorWhite.y;
    printer->outlineColor[2] = g_colorWhite.z;
    printer->outlineColor[3] = g_colorWhite.w;
    printer = self->helpPrinter;
    GfxSpriteLayerDraw(&printer->layer, GfxNewRenderPacket(4000.0f));
  }
}
