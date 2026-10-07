// bdc 0x089a7580 UiMainMenuShowItemLabel
#include "bdc.h"

/* Shows the label sprite of `item` (layout sprite 10+item) when `show` is set and the item is
   unlocked (`unlockedMask` bit `item`): centres its pivot, sets flag 0x20, resets scale 1/angle 0
   (`GfxSpriteSetScaleRotation`) and alpha 0, and resets its fade-in tween `slots[10+item]`
   (t, startAlpha, toggle07 = 0). Otherwise hides it (clears flag 1). */

void UiMainMenuShowItemLabel(UiMainMenu *self, u8 show, u8 item)
{
  GfxSprite *sprite;
  UiTween *tween;

  sprite = ((GfxSprite **)self->base.data)[10 + item];
  if (show == 0) {
    sprite->flags &= ~1u;
  }
  else if ((self->unlockedMask & (1 << item)) == 0) {
    sprite->flags &= ~1u;
  }
  else {
    sprite->flags |= 1;
    GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[10 + item]);
    ((GfxSprite **)self->base.data)[10 + item]->flags |= 0x20;
    ((GfxSprite **)self->base.data)[10 + item]->scaleX = 1.0f;
    ((GfxSprite **)self->base.data)[10 + item]->scaleY = 1.0f;
    ((GfxSprite **)self->base.data)[10 + item]->angle = 0.0f;
    sprite = ((GfxSprite **)self->base.data)[10 + item];
    GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
    ((GfxSprite **)self->base.data)[10 + item]->alpha = 0.0f;
    tween = &self->slots[10 + item].tween;
    tween->t = 0.0f;
    tween->startAlpha = 0.0f;
    tween->toggle07 = 0;
  }
  return;
}
