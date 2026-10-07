// bdc 0x0897aee8 UiCollectionSphereStartArrowTween
#include "bdc.h"

/* Starts the appear (`out` = 0) or disappear tween (`UiTweenBegin`, start scale 1.0, flags 3)
   of the page arrow sprites 0x15..0x1a of `UiCollectionSphere` on the
   tween records `tweens[0x15..0x1a]`. Appearing also makes each sprite visible (flag bit 0),
   mirrors the right-side ones (0x18..0x1a, `GfxSpriteFlipU`), tints 0x15 and 0x18 blue
   (0, 0.5, 1) with alpha 0, sets scale 1 x 2 / angle 0 (`UiSpriteSetScaleRotation`) and
   finally dims the arrows at the page limits with alpha 0 (`UiCollectionSphereDimPageArrows`). */

void UiCollectionSphereStartArrowTween(UiCollectionSphere *self, u8 out)
{
  GfxSprite *sprite;
  s32 i;

  if (out == 0) {
    for (i = 0x15; i < 0x1b; i++) {
      sprite = ((GfxSprite **)self->base.data)[i];
      sprite->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[i];
      if (i >= 0x18) {
        GfxSpriteFlipU(sprite);
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      if (i == 0x15 || i == 0x18) {
        sprite->tint[0] = 0.0f;
        sprite->tint[1] = 0.5f;
        sprite->tint[2] = 1.0f;
        sprite->alpha = 0.0f;
        sprite = ((GfxSprite **)self->base.data)[i];
      }
      UiTweenBegin(1.0f, out, sprite, &self->tweens[i], 3);
      UiSpriteSetScaleRotation(((GfxSprite **)self->base.data)[i], 1.0f, 2.0f, 0.0f);
    }
    UiCollectionSphereDimPageArrows(0.0f, &self->base);
  } else {
    for (i = 0x15; i < 0x1b; i++) {
      UiTweenBegin(1.0f, out, ((GfxSprite **)self->base.data)[i], &self->tweens[i], 3);
    }
  }
}
