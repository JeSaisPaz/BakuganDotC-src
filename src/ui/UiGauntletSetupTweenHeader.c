// bdc 0x0893341c UiGauntletSetupTweenHeader
#include "bdc.h"

/* Starts the open (`closing` = 0) or close tweens of the header sprites of
   `UiGauntletSetup`: sprites 1–5 pop in/out with `UiTweenBegin` (1.5,
   mode 3, tweens 1–5) and sprite 6 slides 64 px (`UiTweenBeginSlide`, mode 0xb, tween 6).
   On open every sprite is made visible (`flags` bit 0) and sprite 6 gets `layerMask` 2. */

void UiGauntletSetupTweenHeader(UiGauntletSetup *self, u8 closing)
{
  int i;

  if (closing == 0) {
    for (i = 1; i < 6; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBegin(1.5f, closing, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 6; i < 7; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 2;
      UiTweenBeginSlide(1.5f, 64.0f, 0.0f, closing, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 0xb);
    }
  } else {
    for (i = 1; i < 6; i++) {
      UiTweenBegin(1.5f, closing, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 6; i < 7; i++) {
      UiTweenBeginSlide(1.5f, 0.0f, 64.0f, closing, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 0xb);
    }
  }
}
