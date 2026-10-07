// bdc 0x08919050 UiAdvSelectTweenArrows
#include "bdc.h"

/* Starts the tweens of sprites 0x18..0x1b of the adventure partner-select screen
   (`UiAdvSelectCtor`, task 376) (tweens `+0x438` = `tweens[0x18]`): unless `hide` is set,
   resets the tint/alpha of sprite 0x19, dims the arrow sprites 0x1a/0x1b
   (`UiAdvSelectSetArrowLit`), makes all four visible (flag 1) and starts them fading in;
   when `hide` is set it only starts the four tweens fading out. */

void UiAdvSelectTweenArrows(UiAdvSelect *self, u8 hide)
{
  GfxSprite **sprites;
  int i;

  if (hide == 0) {
    for (i = 0x18; i < 0x1c; i++) {
      sprites = (GfxSprite **)self->base.data;
      if (i < 0x1a) {
        if (i > 0x18) {
          sprites[i]->tint[0] = 1.0f;
          sprites[i]->tint[1] = 1.0f;
          sprites[i]->tint[2] = 0.0f;
          sprites[i]->alpha = 0.0f;
        }
      } else {
        UiAdvSelectSetArrowLit(self, sprites[i], false);
      }
      sprites = (GfxSprite **)self->base.data;
      sprites[i]->flags |= 1;
      UiTweenBegin(1.0f, 0, sprites[i], &self->tweens[i], 1);
    }
  } else {
    for (i = 0x18; i < 0x1c; i++) {
      sprites = (GfxSprite **)self->base.data;
      UiTweenBegin(1.0f, hide, sprites[i], &self->tweens[i], 1);
    }
  }
}
