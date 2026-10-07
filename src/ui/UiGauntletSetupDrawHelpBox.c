// bdc 0x0893200c UiGauntletSetupDrawHelpBox
#include "bdc.h"

/* Draws the card-help text box `+0xed4` of `UiGauntletSetup`: same as
   `UiGauntletSetupDrawNameBox` with the help-box fields (alpha `+0xed8`/`+0xedc`, glyph count
   `+0xee0`, glyph list `+0xef4`) and outline colour `g_colorWhite`. */

void UiGauntletSetupDrawHelpBox(UiGauntletSetup *self)
{
  UiTextPrinter *printer;
  GfxSprite *spr;
  int i;

  if (self->helpPrinter == NULL) {
    return;
  }
  if (self->helpReveal != 0.0f && self->helpAlpha != self->helpAppliedAlpha) {
    spr = self->helpGlyphs;
    i = 0;
    if (0.0f < self->helpReveal) {
      do {
        i++;
        spr->alpha = self->helpAlpha;
        spr = spr->next;
      } while ((float)i < self->helpReveal);
    }
    self->helpAppliedAlpha = self->helpAlpha;
  }
  printer = self->helpPrinter;
  printer->outlineColor[0] = g_colorWhite.x;
  printer->outlineColor[1] = g_colorWhite.y;
  printer->outlineColor[2] = g_colorWhite.z;
  printer->outlineColor[3] = g_colorWhite.w;
  printer = self->helpPrinter;
  GfxSpriteLayerDraw(&printer->layer, GfxNewRenderPacket(4000.0f));
}
