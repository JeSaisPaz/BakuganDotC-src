// bdc 0x0892930c UiHologramViewDrawText
#include "bdc.h"

/* Draws the help text of the hologram detail view (`UiHologramViewCtor`, task 392; view kind
   `+0x485`): when the text alpha `+0x4c4` changed (`+0x4c8`), applies it to the `+0x4cc` glyph
   sprites of the printer list `+0x4e0`, then draws the printer's sprite layer in a render packet
   with the colour `g_colorWhite`. */

void UiHologramViewDrawText(UiHologramView *self)
{
  GfxSprite *spr;
  int i;
  UiTextPrinter *printer;
  void *packet;

  if (self->glyphCount != 0.0f && self->shownAlpha != self->textAlpha) {
    spr = self->glyphs;
    for (i = 0; (float)i < self->glyphCount; i++) {
      spr->alpha = self->textAlpha;
      spr = spr->next;
    }
    self->shownAlpha = self->textAlpha;
  }
  self->printer->outlineColor[0] = g_colorWhite.x;
  self->printer->outlineColor[1] = g_colorWhite.y;
  self->printer->outlineColor[2] = g_colorWhite.z;
  self->printer->outlineColor[3] = g_colorWhite.w;
  printer = self->printer;
  packet = GfxNewRenderPacket(3000.0f);
  GfxSpriteLayerDraw(&printer->layer, packet);
}
