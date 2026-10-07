// bdc 0x089748e4 UiCollectionMenuStartTitleSlide
#include "bdc.h"

/* Starts the horizontal slide of the title sprites 0x12/0x13 of
   `UiCollectionMenu` (`UiTweenBeginSlide` with flags 7, tweens 0x12/0x13):
   `out` = 0 shows the sprites (flags bit 0) and slides them in from x -128; otherwise slides them
   out to -128. */

void UiCollectionMenuStartTitleSlide(UiCollectionMenu *self, u8 out)
{
  int i;

  if (out == 0) {
    for (i = 0x12; i < 0x14; i++) {
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      UiTweenBeginSlide(1.0f, -128.0f, 0.0f, out, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 7);
    }
  }
  else {
    for (i = 0x12; i < 0x14; i++) {
      UiTweenBeginSlide(1.0f, 0.0f, -128.0f, out, ((GfxSprite **)self->base.data)[i],
                        &self->tweens[i], 7);
    }
  }
}
