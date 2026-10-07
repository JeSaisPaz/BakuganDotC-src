// bdc 0x089381f0 UiUnlockResultDrawNameText
#include "bdc.h"

/* Draws the reward-name text printer `+0x638` of `UiUnlockResult`: when its
   alpha is dirty (`+0x778`) writes alpha `+0x740` to the `+0x750` glyph sprites (list `+0x758`),
   sets the colour from `0x08b00860..6c` and submits it (`GfxNewRenderPacket``(3000)`,
   `GfxSpriteLayerDraw`). */

void UiUnlockResultDrawNameText(UiUnlockResult *self)
{
  UiTextPrinter *printer = self->printers[0];
  void *packet;

  if (printer != NULL) {
    if (self->textReveal[0] != 0.0f && self->textVisible[0] != '\0') {
      GfxSprite *glyph = self->textGlyphs[0];
      int i = 0;
      if (0.0f < self->textReveal[0]) {
        do {
          i++;
          glyph->alpha = self->textAlpha[0];
          glyph = glyph->next;
        } while ((float)i < self->textReveal[0]);
      }
      self->textVisible[0] = '\0';
      printer = self->printers[0];
    }
    printer->outlineColor[0] = g_colorYellow.x;
    printer->outlineColor[1] = g_colorYellow.y;
    printer->outlineColor[2] = g_colorYellow.z;
    printer->outlineColor[3] = g_colorYellow.w;
    printer = self->printers[0];
    packet = GfxNewRenderPacket(3000.0f);
    GfxSpriteLayerDraw(&printer->layer, packet);
  }
}
