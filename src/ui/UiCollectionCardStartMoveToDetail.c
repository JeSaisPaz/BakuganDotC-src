// bdc 0x08986328 UiCollectionCardStartMoveToDetail
#include "bdc.h"

/* Prepares the move of the selected card art (sprite 13 + cursor) of
   `UiCollectionCard` into the detail view (`back` = 0) or back: fills its
   tween record (tweens[13 + cursor]) with the from/to positions (saved cell position `bobPos`
   <-> detail anchor sprite 0x34), the offsets `detailOffsetX/Y` (to - from) and the scale range
   (0.3 -> 0.8 into the detail view, 0.8 -> 0.3 back), and resets its progress. Going into the
   detail view also puts the card sprite on draw layer 8. */

void UiCollectionCardStartMoveToDetail(UiCollectionCard *self, u8 back)
{
  s32 i = self->cursor;
  UiTween *tween = &self->tweens[13 + i];
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  s16 cellX = (s16)(s32)self->bobPos[i][0];
  s16 cellY = (s16)(s32)self->bobPos[i][1];

  if (!back) {
    /* from the cell (slideStart, slideEnd) to the anchor (slideDelta, moveToY) */
    tween->slideStart = cellX;
    tween->slideEnd = cellY;
    tween->slideDelta = (s16)(s32)sprites[0x34]->posX;
    tween->moveToY = (s16)(s32)sprites[0x34]->posY;
    self->detailScale = 0.3f;
    self->detailOffsetX = (float)(tween->slideDelta - tween->slideStart);
    self->detailScaleEnd = 0.8f;
    tween->t = 0.0f;
    self->detailOffsetY = (float)(tween->moveToY - tween->slideEnd);
    sprites[13 + i]->layerMask = 8;
    return;
  }
  /* from the anchor back to the cell */
  tween->slideDelta = cellX;
  tween->moveToY = cellY;
  tween->slideStart = (s16)(s32)sprites[0x34]->posX;
  tween->slideEnd = (s16)(s32)sprites[0x34]->posY;
  self->detailScaleEnd = 0.3f;
  self->detailOffsetX = (float)(tween->slideDelta - tween->slideStart);
  self->detailScale = 0.8f;
  tween->t = 0.0f;
  self->detailOffsetY = (float)(tween->moveToY - tween->slideEnd);
}
