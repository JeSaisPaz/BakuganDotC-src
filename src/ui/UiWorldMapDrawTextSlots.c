// bdc 0x0899728c UiWorldMapDrawTextSlots
#include "bdc.h"

/* Draws the three text slots of `UiWorldMap` (`textSlot[]`, stride 0x224),
   returning at the first slot without a printer. For each slot: when it has glyphs
   (`glyphCount` != 0) and `alpha` differs from `drawnAlpha`, applies `alpha` to every glyph
   sprite of the `glyphs` list and stores it in `drawnAlpha`; then sets the printer's outline
   colour to `g_colorWhite` and draws the printer's sprite layer into a new render packet at
   depth 400 (`GfxNewRenderPacket`, `GfxSpriteLayerDraw`). */

void UiWorldMapDrawTextSlots(UiScreen *screen)

{
  UiWorldMap *map = (UiWorldMap *)screen;
  UiTextPrinter *printer;
  GfxSprite *glyph;
  int i;
  int n;

  for (i = 0; i < 3; i++) {
    if (map->textSlot[i].printer == NULL) {
      return;
    }
    if (map->textSlot[i].glyphCount != 0.0f &&
        map->textSlot[i].alpha != map->textSlot[i].drawnAlpha) {
      glyph = map->textSlot[i].glyphs;
      n = 0;
      if (0.0f < map->textSlot[i].glyphCount) {
        do {
          n++;
          glyph->alpha = map->textSlot[i].alpha;
          glyph = glyph->next;
        } while ((float)n < map->textSlot[i].glyphCount);
      }
      map->textSlot[i].drawnAlpha = map->textSlot[i].alpha;
    }
    printer = map->textSlot[i].printer;
    printer->outlineColor[0] = g_colorWhite.x;
    printer->outlineColor[1] = g_colorWhite.y;
    printer->outlineColor[2] = g_colorWhite.z;
    printer->outlineColor[3] = g_colorWhite.w;
    printer = map->textSlot[i].printer;
    GfxSpriteLayerDraw(&printer->layer, GfxNewRenderPacket(400.0f));
  }
}
