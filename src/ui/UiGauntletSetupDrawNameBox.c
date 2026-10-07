// bdc 0x08931f38 UiGauntletSetupDrawNameBox
#include "bdc.h"

/* Draws the card-name text box `+0xcb0` of `UiGauntletSetup`: when the wanted
   alpha `+0xcb4` differs from the applied one `+0xcb8`, writes it to the first `+0xcbc` glyph
   sprites of the printer's list (`+0xcd0`), sets the box outline colour from `g_colorGreen` and submits
   it (`GfxNewRenderPacket``(4000)`, `GfxSpriteLayerDraw`). */

void UiGauntletSetupDrawNameBox(UiGauntletSetup *self)
{
  UiTextPrinter *printer;
  GfxSprite *spr;
  int i;

  if (self->namePrinter == NULL) {
    return;
  }
  if (self->nameReveal != 0.0f && self->nameAlpha != self->nameAppliedAlpha) {
    spr = self->nameGlyphs;
    i = 0;
    if (0.0f < self->nameReveal) {
      do {
        i++;
        spr->alpha = self->nameAlpha;
        spr = spr->next;
      } while ((float)i < self->nameReveal);
    }
    self->nameAppliedAlpha = self->nameAlpha;
  }
  printer = self->namePrinter;
  printer->outlineColor[0] = g_colorGreen.x;
  printer->outlineColor[1] = g_colorGreen.y;
  printer->outlineColor[2] = g_colorGreen.z;
  printer->outlineColor[3] = g_colorGreen.w;
  printer = self->namePrinter;
  GfxSpriteLayerDraw(&printer->layer, GfxNewRenderPacket(4000.0f));
}
