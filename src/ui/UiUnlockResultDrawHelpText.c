// bdc 0x089382b8 UiUnlockResultDrawHelpText
#include "bdc.h"

/* Draws the help text printer `+0x63c` of `UiUnlockResult`: same as
   `UiUnlockResultDrawNameText` with alpha `+0x744`, dirty flag `+0x779`, glyph count `+0x754`,
   list `+0x75c` and colour `0x08b00190..9c`. */

void UiUnlockResultDrawHelpText(UiUnlockResult *self)
{
  UiTextPrinter *printer = self->printers[1];
  void *packet;

  if (printer != NULL) {
    if (self->textReveal[1] != 0.0f && self->textVisible[1] != '\0') {
      GfxSprite *glyph = self->textGlyphs[1];
      int i = 0;
      if (0.0f < self->textReveal[1]) {
        do {
          i++;
          glyph->alpha = self->textAlpha[1];
          glyph = glyph->next;
        } while ((float)i < self->textReveal[1]);
      }
      self->textVisible[1] = '\0';
      printer = self->printers[1];
    }
    printer->outlineColor[0] = g_colorWhite.x;
    printer->outlineColor[1] = g_colorWhite.y;
    printer->outlineColor[2] = g_colorWhite.z;
    printer->outlineColor[3] = g_colorWhite.w;
    printer = self->printers[1];
    packet = GfxNewRenderPacket(3000.0f);
    GfxSpriteLayerDraw(&printer->layer, packet);
  }
}
