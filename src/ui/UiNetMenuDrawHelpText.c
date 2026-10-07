// bdc 0x0894de90 UiNetMenuDrawHelpText
#include "bdc.h"

/* Draws the help text of `UiNetMenu`: applies alpha `+0x2e0` to the `+0x2ec` glyphs
   (list `+0x304`) when it changed (`+0x2e4`), sets the colour from `g_colorWhite` and submits the
   printer `+0x2dc` (`GfxNewRenderPacket`, `GfxSpriteLayerDraw`). */

void UiNetMenuDrawHelpText(UiScreen *screen)

{
  UiNetMenu *menu = (UiNetMenu *)screen;
  GfxSprite *glyph;
  GfxSprite *printer;
  float *addColor;
  int i;

  if (menu->helpGlyphCount != 0.0f && menu->helpAppliedAlpha != menu->titleHelpAlpha) {
    glyph = menu->helpGlyphs;
    i = 0;
    if (0.0f < menu->helpGlyphCount) {
      do {
        i = i + 1;
        glyph->alpha = menu->titleHelpAlpha;
        glyph = glyph->next;
      } while ((float)i < menu->helpGlyphCount);
    }
    menu->helpAppliedAlpha = menu->titleHelpAlpha;
  }
  addColor = menu->helpPrinter->addColor;
  addColor[0] = g_colorWhite.x;
  addColor[1] = g_colorWhite.y;
  addColor[2] = g_colorWhite.z;
  addColor[3] = g_colorWhite.w;
  printer = menu->helpPrinter;
  GfxSpriteLayerDraw((GfxSpriteLayer *)printer, GfxNewRenderPacket(400.0f));
}
