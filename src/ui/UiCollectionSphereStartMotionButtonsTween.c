// bdc 0x0897deb8 UiCollectionSphereStartMotionButtonsTween
#include "bdc.h"

/* Starts the appear (`out` = 0) or disappear tween (`UiTweenBegin`, start scale 1.0, flags 3)
   of the motion buttons of the detail view of the sphere (Bakugan figure) collection screen
   (`UiCollectionSphere`): sprites 0x3a..0x3d and 0x3e..0x41 on the
   tween records `tweens[0x3a..0x41]`. Appearing makes sprite 0x3a+k and 0x3e+k visible (flag
   bit 0) when bit k of `motionButtonMask` is set and hides them otherwise, puts both sets on
   layer mask 8, gives buttons 1..3 the icons 4, 5 and 0 (`UiSetButtonIcon`) and tints the
   second set white with alpha 0. */

void UiCollectionSphereStartMotionButtonsTween(UiCollectionSphere *self, u8 out)
{
  GfxSprite *sprite;
  s32 i;
  s32 k;

  if (out == 0) {
    for (i = 0x3a; i < 0x3e; i++) {
      k = i - 0x3a;
      sprite = ((GfxSprite **)self->base.data)[i];
      if (self->motionButtonMask & (1 << k)) {
        sprite->flags |= 1;
      } else {
        sprite->flags &= ~1u;
      }
      sprite = ((GfxSprite **)self->base.data)[i];
      if (k == 1) {
        UiSetButtonIcon(sprite, 4);
        sprite = ((GfxSprite **)self->base.data)[i];
      } else if (k == 2) {
        UiSetButtonIcon(sprite, 5);
        sprite = ((GfxSprite **)self->base.data)[i];
      } else if (k == 3) {
        UiSetButtonIcon(sprite, 0);
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      sprite->layerMask = 8;
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x3e; i < 0x42; i++) {
      k = i - 0x3e;
      sprite = ((GfxSprite **)self->base.data)[i];
      if (self->motionButtonMask & (1 << k)) {
        sprite->flags |= 1;
      } else {
        sprite->flags &= ~1u;
      }
      ((GfxSprite **)self->base.data)[i]->layerMask = 8;
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->tint[0] = 1.0f;
      sprite->tint[1] = 1.0f;
      sprite->tint[2] = 1.0f;
      sprite->alpha = 0.0f;
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  } else {
    for (i = 0x3a; i < 0x3e; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
    for (i = 0x3e; i < 0x42; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}
