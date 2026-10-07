// bdc 0x0898c524 UiCollectionFigureStartBgTween
#include "bdc.h"

/* Starts the fade tween (`UiTweenBegin`, records `tweens[0x23..0x27]`) of the background sprites
   0x23..0x27 of `UiCollectionFigure`. When opening (`out == 0`) each sprite
   is first made visible and put on draw layer 8, and sprites 0x24/0x25 get button icons 2/1
   (`UiSetButtonIcon`). */

void UiCollectionFigureStartBgTween(UiCollectionFigure *self, u8 out)
{
  UiTween *tween;
  int i;

  if (out == 0) {
    tween = &self->tweens[0x23];
    for (i = 0x23; i < 0x28; i++) {
      if (i == 0x24) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 2);
      } else if (i == 0x25) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 1);
      }
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 8;
      UiTweenBegin(1.0f, 0, ((GfxSprite **)self->base.data)[i], tween, 3);
      tween++;
    }
  } else {
    tween = &self->tweens[0x23];
    for (i = 0x23; i < 0x28; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], tween, 3);
      tween++;
    }
  }
}
