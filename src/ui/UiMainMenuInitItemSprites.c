// bdc 0x089a6e38 UiMainMenuInitItemSprites
#include "bdc.h"

/* Without `closing`: centres the pivots of layout sprites 5..14 (the five item sprites and their
   labels, `GfxSpriteCenterPivot`), sets flag 0x20 and resets them to scale 1, angle 0
   (`GfxSpriteSetScaleRotation`); remembers item sprite 5's position in `itemHomeX/itemHomeY`.
   Then shows the five item sprites (flag 1) with a white tint when the item is unlocked
   (`unlockedMask` bit) or grey 0.6 when locked, alpha 1 for the cursor item and 0.4 for the
   others, scale 1.5, angle 0, parks them at x -224 and primes their slide tweens `slots[5+i]`
   (delay `2*i`, toggle07 0, t 0, slideStart = x, slideDelta = 704 - x).
   With `closing` set: hides the cursor item's label (sprite 10+cursor, clears flag 1) and snapshots
   the cursor item sprite's alpha and scaleX into its tween `slots[5+cursor]` (t 0). */

void UiMainMenuInitItemSprites(UiMainMenu *self, u8 closing)

{
  GfxSprite *sprite;
  UiTween *tween;
  int i;
  u32 unlocked;

  if (closing == 0) {
    for (i = 5; i < 15; i++) {
      GfxSpriteCenterPivot(((GfxSprite **)self->base.data)[i]);
      ((GfxSprite **)self->base.data)[i]->flags |= 0x20;
      ((GfxSprite **)self->base.data)[i]->scaleX = 1.0f;
      ((GfxSprite **)self->base.data)[i]->scaleY = 1.0f;
      ((GfxSprite **)self->base.data)[i]->angle = 0.0f;
      sprite = ((GfxSprite **)self->base.data)[i];
      GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
    }
    self->itemHomeX = ((GfxSprite **)self->base.data)[5]->posX;
    self->itemHomeY = ((GfxSprite **)self->base.data)[5]->posY;
    for (i = 0; i < 5; i++) {
      ((GfxSprite **)self->base.data)[5 + i]->flags |= 1;
      sprite = ((GfxSprite **)self->base.data)[5 + i];
      unlocked = self->unlockedMask & (1 << i);
      if (self->cursor == i) {
        if (unlocked != 0) {
          sprite->tint[0] = 1.0f;
          sprite->tint[1] = 1.0f;
          sprite->tint[2] = 1.0f;
          sprite->alpha = 1.0f;
        }
        else {
          sprite->tint[0] = 0.6f;
          sprite->tint[1] = 0.6f;
          sprite->tint[2] = 0.6f;
          sprite->alpha = 1.0f;
        }
      }
      else if (unlocked != 0) {
        sprite->tint[0] = 1.0f;
        sprite->tint[1] = 1.0f;
        sprite->tint[2] = 1.0f;
        sprite->alpha = 0.4f;
      }
      else {
        sprite->tint[0] = 0.6f;
        sprite->tint[1] = 0.6f;
        sprite->tint[2] = 0.6f;
        sprite->alpha = 0.4f;
      }
      ((GfxSprite **)self->base.data)[5 + i]->scaleX = 1.5f;
      ((GfxSprite **)self->base.data)[5 + i]->scaleY = 1.5f;
      ((GfxSprite **)self->base.data)[5 + i]->angle = 0.0f;
      sprite = ((GfxSprite **)self->base.data)[5 + i];
      GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
      ((GfxSprite **)self->base.data)[5 + i]->posX = -224.0f;
      tween = &self->slots[5 + i].tween;
      tween->delay0b = (u8)(i * 2);
      tween->toggle07 = 0;
      tween->t = 0.0f;
      tween->slideStart = (s16)(int)((GfxSprite **)self->base.data)[5 + i]->posX;
      tween->slideDelta = (s16)(int)(704.0f - ((GfxSprite **)self->base.data)[5 + i]->posX);
    }
  }
  else {
    ((GfxSprite **)self->base.data)[10 + self->cursor]->flags &= ~1u;
    tween = &self->slots[5 + self->cursor].tween;
    tween->t = 0.0f;
    tween->startAlpha = ((GfxSprite **)self->base.data)[5 + self->cursor]->alpha;
    tween->startScale = ((GfxSprite **)self->base.data)[5 + self->cursor]->scaleX;
  }
}
