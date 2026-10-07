// bdc 0x089883b0 UiCollectionTheaterStartBgTween
#include "bdc.h"

/* Starts the fade tween (`UiTweenBegin`, records `bgTweens`) of the background sprites 0x29..0x2d
   of `UiCollectionTheater`. When opening (`out == 0`) each sprite is first
   made visible and put on draw layer 4, and sprites 0x2a/0x2b get button icons 2/1
   (`UiSetButtonIcon`). */

void UiCollectionTheaterStartBgTween(UiScreen *screen, u8 out)
{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  UiTween *tween;
  int i;

  if (out == 0) {
    tween = self->bgTweens;
    for (i = 0x29; i < 0x2e; i++) {
      if (i == 0x2a) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 2);
      } else if (i == 0x2b) {
        UiSetButtonIcon(((GfxSprite **)self->base.data)[i], 1);
      }
      ((GfxSprite **)self->base.data)[i]->flags |= 1;
      ((GfxSprite **)self->base.data)[i]->layerMask = 4;
      UiTweenBegin(1.0f, 0, ((GfxSprite **)self->base.data)[i], tween, 3);
      tween++;
    }
  } else {
    tween = self->bgTweens;
    for (i = 0x29; i < 0x2e; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], tween, 3);
      tween++;
    }
  }
}
