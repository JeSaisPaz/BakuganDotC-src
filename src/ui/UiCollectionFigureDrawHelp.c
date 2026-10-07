// bdc 0x0898b66c UiCollectionFigureDrawHelp
#include "bdc.h"

/* Draws the description printer of `UiCollectionFigure`, first copying
   the help alpha into its glyph sprites when it changed. Sets the outline colour to g_colorWhite
  . */

void UiCollectionFigureDrawHelp(UiCollectionFigure *self)
{
  UiTextPrinter *printer;
  GfxSprite *sprite;
  void *packet;
  s32 i;

  if (self->helpPrinter != NULL) {
    if (self->textLen != 0.0f && self->helpAlpha != self->fadeFrom) {
      sprite = self->glyphs;
      i = 0;
      if (0.0f < self->textLen) {
        do {
          i++;
          sprite->alpha = self->helpAlpha;
          sprite = sprite->next;
        } while ((float)i < self->textLen);
      }
      self->fadeFrom = self->helpAlpha;
    }
    self->helpPrinter->outlineColor[0] = g_colorWhite.x;
    self->helpPrinter->outlineColor[1] = g_colorWhite.y;
    self->helpPrinter->outlineColor[2] = g_colorWhite.z;
    self->helpPrinter->outlineColor[3] = g_colorWhite.w;
    printer = self->helpPrinter;
    packet = GfxNewRenderPacket(320.0f);
    GfxSpriteLayerDraw(&printer->layer, packet);
  }
}
