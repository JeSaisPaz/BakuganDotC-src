// bdc 0x0898e678 UiCollectionFigureHelpTextFadeDone
#include "bdc.h"

/* Advances the description-text fade of `UiCollectionFigure` by 1/16
   (ease-out on alpha `+0xeac`); when fading out it finally clears the line count `+0xeb4` and the
   text object `+0xea8` (`GfxSpriteLayerClear`). Returns 1 when done. */

u8 UiCollectionFigureHelpTextFadeDone(UiCollectionFigure *self, u8 out)
{
  u8 done;
  UiTextPrinter *printer;
  float fade;
  float alpha;

  fade = self->descFade + 0.0625f;
  done = 0;
  alpha = self->helpAlpha;
  if (out == 0) {
    self->descFade = fade;
    self->helpAlpha = alpha + (1.0f - (fade - 1.0f) * (fade - 1.0f));
    if (!(fade < 1.0f)) {
      self->helpAlpha = 1.0f;
      done = 1;
    }
  } else {
    self->descFade = fade;
    self->helpAlpha = alpha - (1.0f - (fade - 1.0f) * (fade - 1.0f));
    if (!(fade < 1.0f)) {
      printer = self->helpPrinter;
      self->textLen = 0.0f;
      GfxSpriteLayerClear(&printer->layer);
      printer->glyphs = (GfxSprite *)0;
      done = 1;
    }
  }
  return done;
}
