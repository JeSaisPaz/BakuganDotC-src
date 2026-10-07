// bdc 0x08957a50 UiEquipDrawNameText
#include "bdc.h"

/* Draws the name text printer `namePrinter` of `UiEquip`: when dirty (`nameDirty`) and
   `nameGlyphCount` is non-zero, applies `nameAlpha` to the glyph list `nameGlyphs`; then copies
   `g_colorGreen` into the printer's outline colour (one VFPU quad copy) and submits it
   (`GfxNewRenderPacket``(4000)`, `GfxSpriteLayerDraw`). */

void UiEquipDrawNameText(UiEquip *self)

{
  UiTextPrinter *printer;
  GfxSprite *glyph;
  int i;

  if (self->namePrinter != NULL) {
    if (self->nameGlyphCount != 0.0f && self->nameDirty != 0) {
      glyph = self->nameGlyphs;
      i = 0;
      if (0.0f < self->nameGlyphCount) {
        do {
          i++;
          glyph->alpha = self->nameAlpha;
          glyph = glyph->next;
        } while ((float)i < self->nameGlyphCount);
      }
      self->nameDirty = 0;
    }
    printer = self->namePrinter;
    printer->outlineColor[0] = g_colorGreen.x;
    printer->outlineColor[1] = g_colorGreen.y;
    printer->outlineColor[2] = g_colorGreen.z;
    printer->outlineColor[3] = g_colorGreen.w;
    printer = self->namePrinter;
    GfxSpriteLayerDraw(&printer->layer, GfxNewRenderPacket(4000.0f));
  }
}
