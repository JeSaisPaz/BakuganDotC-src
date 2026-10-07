// bdc 0x089697ac UiCardEquipDrawCardName
#include "bdc.h"

/* Draws the card-name text printer of `UiCardEquip` at depth 4000: when the name changed (nameDirty) first copies nameAlpha into every glyph sprite, then sets the printer's outline colour to g_colorYellow and queues it (`GfxNewRenderPacket`, `GfxSpriteLayerDraw`). */

void UiCardEquipDrawCardName(UiCardEquip *self)
{
  UiTextPrinter *printer;
  GfxSprite *sprite;
  void *packet;
  float *outline;
  s32 i;

  if (self->namePrinter != NULL) {
    if (self->nameGlyphCount != 0.0f && self->nameDirty != 0) {
      sprite = self->nameGlyphs;
      i = 0;
      if (0.0f < self->nameGlyphCount) {
        do {
          i++;
          sprite->alpha = self->nameAlpha;
          sprite = sprite->next;
        } while ((float)i < self->nameGlyphCount);
      }
      self->nameDirty = 0;
    }
    /* copies g_colorYellow into the printer outline colour (lv.q/sv.q) */
    outline = self->namePrinter->outlineColor;
    outline[0] = g_colorYellow.x;
    outline[1] = g_colorYellow.y;
    outline[2] = g_colorYellow.z;
    outline[3] = g_colorYellow.w;
    printer = self->namePrinter;
    packet = GfxNewRenderPacket(4000.0f);
    GfxSpriteLayerDraw(&printer->layer, packet);
  }
}
