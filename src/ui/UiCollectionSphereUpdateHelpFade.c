// bdc 0x0897d1d8 UiCollectionSphereUpdateHelpFade
#include "bdc.h"

/* Advances the description fade of `UiCollectionSphere` by 1/16 per frame;
   when fading out finishes, clears the printer. Returns 1 when finished. */

u8 UiCollectionSphereUpdateHelpFade(UiCollectionSphere *self, u8 out)

{
  u8 done;
  UiTextPrinter *printer;
  float t;
  
  t = self->descFade + 0.0625f;
  done = 0;
  if (out == 0) {
    self->descFade = t;
    self->helpAlpha = self->helpAlpha + (1.0f - (t - 1.0f) * (t - 1.0f));
    if (!(t < 1.0f)) {
      self->helpAlpha = 1.0f;
      done = 1;
    }
  }
  else {
    self->descFade = t;
    self->helpAlpha = self->helpAlpha - (1.0f - (t - 1.0f) * (t - 1.0f));
    if (!(t < 1.0f)) {
      printer = self->helpPrinter;
      self->helpHeight = 0.0f;
      GfxSpriteLayerClear(&printer->layer);
      printer->glyphs = (GfxSprite *)0x0;
      done = 1;
    }
  }
  return done;
}

