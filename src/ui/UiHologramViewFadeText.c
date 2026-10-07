// bdc 0x08929aa0 UiHologramViewFadeText
#include "bdc.h"

/* Fades the help text glyphs of the hologram detail view (`UiHologramViewCtor`, task 392; view
   kind `+0x485`) in (`out == 0`) or out by an eased step (`t` `+0x4d0` += 1/8) on their alpha
   `+0xbc`; returns true when finished. */

bool UiHologramViewFadeText(UiHologramView *self, char out)
{
  GfxSprite *spr;
  int i;

  self->fadeT = self->fadeT + 0.125f;
  spr = self->glyphs;
  for (i = 0; (float)i < self->glyphCount; i++) {
    if (out == '\0') {
      spr->alpha = self->textAlpha + (1.0f - (self->fadeT - 1.0f) * (self->fadeT - 1.0f));
    } else {
      spr->alpha = self->textAlpha - self->fadeT * self->fadeT;
    }
    spr = spr->next;
  }
  return !(self->fadeT < 1.0f);
}
