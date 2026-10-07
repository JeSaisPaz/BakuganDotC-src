// bdc 0x08988e38 UiCollectionTheaterStartArrowTween
#include "bdc.h"

/* Starts the fade tween (`UiTweenBegin`, arrowTweens[0..5]) of the page arrow sprites 0x19..0x1e
   of `UiCollectionTheater`. On fade-in (`out == 0`) it first sets bit 0
   of each sprite's flags, mirrors the right side (0x1c..0x1e), gives the outer ones (0x19, 0x1c) a
   blue tint at alpha 0, stretches them to scale (1, 2) after starting the tween, and finally dims
   the limit arrows (`UiCollectionTheaterDimPageArrows`) to alpha 0. */

void UiCollectionTheaterStartArrowTween(UiScreen *screen, u8 out)
{
  UiCollectionTheater *self = (UiCollectionTheater *)screen;
  GfxSprite *sprite;
  int i;

  if (out == 0) {
    for (i = 0x19; i < 0x1f; i++) {
      sprite = ((GfxSprite **)screen->data)[i];
      sprite->flags |= 1;
      sprite = ((GfxSprite **)screen->data)[i];
      if (i >= 0x1c) {
        GfxSpriteFlipU(sprite);
        sprite = ((GfxSprite **)screen->data)[i];
      }
      if (i == 0x19 || i == 0x1c) {
        sprite->tint[0] = 0.0f;
        sprite->tint[1] = 0.5f;
        sprite->tint[2] = 1.0f;
        sprite->alpha = 0.0f;
        sprite = ((GfxSprite **)screen->data)[i];
      }
      UiTweenBegin(1.0f, out, sprite, &self->arrowTweens[i - 0x19], 3);
      UiSpriteSetScaleRotation(((GfxSprite **)screen->data)[i], 1.0f, 2.0f, 0.0f);
    }
    UiCollectionTheaterDimPageArrows(0.0f, screen);
  } else {
    for (i = 0x19; i < 0x1f; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)screen->data)[i], &self->arrowTweens[i - 0x19], 3);
    }
  }
}
