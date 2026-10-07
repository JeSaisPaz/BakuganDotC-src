// bdc 0x08982838 UiCollectionCardDrawHelp
#include "bdc.h"

/* Draws the card description printer of `UiCollectionCard`, first copying
   the help alpha into its glyph sprites when it changed. Sets the outline colour to g_colorWhite
  . */

void UiCollectionCardDrawHelp(UiCollectionCard *self)
{
  UiTextPrinter *printer;
  GfxSprite *sprite;
  void *packet;
  s32 i;

  if (self->helpPrinter != NULL) {
    if (self->helpHeight != 0.0f && self->helpAlpha != self->helpAlphaApplied) {
      sprite = self->helpGlyphs;
      i = 0;
      if (0.0f < self->helpHeight) {
        do {
          i++;
          sprite->alpha = self->helpAlpha;
          sprite = sprite->next;
        } while ((float)i < self->helpHeight);
      }
      self->helpAlphaApplied = self->helpAlpha;
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
