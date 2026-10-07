// bdc 0x0898648c UiCollectionCardMoveCardToDetail
#include "bdc.h"

/* Moves the selected card art (sprite 13 + cursor, tween record tweens[13 + cursor]) of the card
   collection screen (task 313, `maybe_UiScreen313Ctor`; pages of ability cards
   `"collection_ability_%02d"` in `"waku_4_a"`/`"waku_4_b"` frames, large card art `"card_L_%03d"`,
   help text `"DWCardHelp"`) one step (1/16) between its grid cell and the detail position set up
   by `UiCollectionCardStartMoveToDetail`: position from (slideStart, slideEnd) plus
   `detailOffsetX/Y`, scale from `detailScale` to `detailScaleEnd`. `back` = 0 eases out towards the
   detail view (1 - (t-1)^2), `back` != 0 eases in back to the cell (t^2). Once t reaches 1 the
   sprite snaps to (slideDelta, moveToY) at `detailScaleEnd` (going back it also returns to draw
   layer 1). The sprite's scaleY is set to scaleX, rotation to 0, and its matrix rebuilt. Returns
   true when the move is done (t >= 1), false otherwise. */

bool UiCollectionCardMoveCardToDetail(UiCollectionCard *self, bool back)
{
  UiTween *tween = &self->tweens[13 + self->cursor];
  GfxSprite *sprite;
  float t = tween->t + 0.0625f;
  float k;
  bool done = false;

  if (!back) {
    /* ease out: 1 - (t - 1)^2 */
    tween->t = t;
    sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
    sprite->posX = (float)tween->slideStart + (1.0f - (t - 1.0f) * (t - 1.0f)) * self->detailOffsetX;
    tween = &self->tweens[13 + self->cursor];
    k = tween->t - 1.0f;
    sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
    sprite->posY = (float)tween->slideEnd + (1.0f - k * k) * self->detailOffsetY;
    tween = &self->tweens[13 + self->cursor];
    k = tween->t - 1.0f;
    sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
    sprite->scaleX = self->detailScale +
                     (1.0f - k * k) * (self->detailScaleEnd - self->detailScale);
    tween = &self->tweens[13 + self->cursor];
    sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
    if (!(tween->t < 1.0f)) {
      sprite->posX = (float)tween->slideDelta;
      done = true;
      tween = &self->tweens[13 + self->cursor];
      ((GfxSprite **)self->base.data)[13 + self->cursor]->posY = (float)tween->moveToY;
      ((GfxSprite **)self->base.data)[13 + self->cursor]->scaleX = self->detailScaleEnd;
      sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
    }
  } else {
    /* ease in: t^2 */
    tween->t = t;
    sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
    sprite->posX = (float)tween->slideStart + t * t * self->detailOffsetX;
    tween = &self->tweens[13 + self->cursor];
    k = tween->t;
    sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
    sprite->posY = (float)tween->slideEnd + k * k * self->detailOffsetY;
    tween = &self->tweens[13 + self->cursor];
    k = tween->t;
    sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
    sprite->scaleX = self->detailScale - k * k * (self->detailScale - self->detailScaleEnd);
    tween = &self->tweens[13 + self->cursor];
    sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
    if (!(tween->t < 1.0f)) {
      sprite->posX = (float)tween->slideDelta;
      tween = &self->tweens[13 + self->cursor];
      ((GfxSprite **)self->base.data)[13 + self->cursor]->posY = (float)tween->moveToY;
      ((GfxSprite **)self->base.data)[13 + self->cursor]->scaleX = self->detailScaleEnd;
      ((GfxSprite **)self->base.data)[13 + self->cursor]->layerMask = 1;
      done = true;
      sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
    }
  }
  sprite->scaleY = sprite->scaleX;
  ((GfxSprite **)self->base.data)[13 + self->cursor]->angle = 0.0f;
  sprite = ((GfxSprite **)self->base.data)[13 + self->cursor];
  GfxSpriteSetScaleRotation(sprite, sprite->scaleX, sprite->scaleY, sprite->angle, false);
  return done;
}
