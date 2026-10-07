// bdc 0x089795fc UiCollectionSphereDrawHelp
#include "bdc.h"

/* Draws the description printer of `UiCollectionSphere` at depth 320,
   first copying the help alpha `+0xf14` into its glyph sprites when it changed. */

void UiCollectionSphereDrawHelp(UiCollectionSphere *self)

{
  UiTextPrinter *printer;
  GfxSprite *glyph;
  int i;
  void *packet;

  if (self->helpPrinter != NULL) {
    if (self->helpHeight != 0.0f && self->helpAlpha != self->helpAlphaApplied) {
      glyph = self->helpGlyphs;
      i = 0;
      if (0.0f < self->helpHeight) {
        do {
          i = i + 1;
          glyph->alpha = self->helpAlpha;
          glyph = glyph->next;
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
  return;
}
