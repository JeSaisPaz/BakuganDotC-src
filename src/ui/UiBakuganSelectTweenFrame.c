// bdc 0x0892d11c UiBakuganSelectTweenFrame
#include "bdc.h"

/* Starts the frame tweens of the Bakugan select screen (`UiBakuganSelectCtor`, task 371), start
   scale 1.5, `fadeOut = hide`. Show (`hide == 0`): sprites 0x10..0x14 are made visible (flags bit0)
   and fade in (`UiTweenBegin`, flags 3); sprite 0x15 (flags 0xb, X slide) and sprites 0x16..0x17
   (flags 7, Y slide) are made visible, get `layerMask = 2` and slide in from 64 to 0
   (`UiTweenBeginSlide`). Hide: the same sprites run the reverse tweens (slide 0 to 64) without
   touching their flags or layer mask. Each sprite `i` uses `tweens[i]`. */

void UiBakuganSelectTweenFrame(UiBakuganSelect *self, u8 hide)
{
  GfxSprite *sprite;
  int i;

  if (hide == 0) {
    for (i = 0x10; i < 0x15; i++) {
      sprite = ((GfxSprite **)(self->base).data)[i];
      sprite->flags |= 1;
      UiTweenBegin(1.5f, hide, ((GfxSprite **)(self->base).data)[i], &self->tweens[i], 3);
    }
    for (i = 0x15; i < 0x16; i++) {
      sprite = ((GfxSprite **)(self->base).data)[i];
      sprite->flags |= 1;
      ((GfxSprite **)(self->base).data)[i]->layerMask = 2;
      UiTweenBeginSlide(1.5f, 64.0f, 0.0f, hide, ((GfxSprite **)(self->base).data)[i],
                        &self->tweens[i], 0xb);
    }
    for (i = 0x16; i < 0x18; i++) {
      sprite = ((GfxSprite **)(self->base).data)[i];
      sprite->flags |= 1;
      ((GfxSprite **)(self->base).data)[i]->layerMask = 2;
      UiTweenBeginSlide(1.5f, 64.0f, 0.0f, hide, ((GfxSprite **)(self->base).data)[i],
                        &self->tweens[i], 7);
    }
  }
  else {
    for (i = 0x10; i < 0x15; i++) {
      UiTweenBegin(1.5f, hide, ((GfxSprite **)(self->base).data)[i], &self->tweens[i], 3);
    }
    for (i = 0x15; i < 0x16; i++) {
      UiTweenBeginSlide(1.5f, 0.0f, 64.0f, hide, ((GfxSprite **)(self->base).data)[i],
                        &self->tweens[i], 0xb);
    }
    for (i = 0x16; i < 0x18; i++) {
      UiTweenBeginSlide(1.5f, 0.0f, 64.0f, hide, ((GfxSprite **)(self->base).data)[i],
                        &self->tweens[i], 7);
    }
  }
}
