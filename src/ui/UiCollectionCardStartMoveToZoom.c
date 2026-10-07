// bdc 0x089868d0 UiCollectionCardStartMoveToZoom
#include "bdc.h"

/* Prepares the move of the opened card (sprite 13 + cursor, tween record 13 + cursor) of
   `UiCollectionCard` between the detail anchor (sprite 52) and the screen
   centre (240, 136): `back` = 0 moves anchor -> centre with scale 0.8 -> 1.0, otherwise centre ->
   anchor with the card's current scale -> 0.8. Stores from/to in the tween's four s16, the offsets
   in `detailOffsetX/Y` and resets the progress `t`. */

void UiCollectionCardStartMoveToZoom(UiCollectionCard *self, u8 back)

{
  int cursor = self->cursor;
  GfxSprite **sprites = (GfxSprite **)self->base.data;
  UiTween *move = &self->tweens[cursor + 13];

  if (back == 0) {
    move->slideStart = (s16)(int)sprites[52]->posX;
    move->slideDelta = 240;
    move->moveToY = 136;
    move->slideEnd = (s16)(int)sprites[52]->posY;
    self->detailScale = 0.8f;
    self->detailOffsetX = (float)(move->slideDelta - move->slideStart);
    move->t = 0.0f;
    self->detailScaleEnd = 1.0f;
    self->detailOffsetY = (float)(move->moveToY - move->slideEnd);
    return;
  }
  move->slideDelta = (s16)(int)sprites[52]->posX;
  move->slideStart = 240;
  move->slideEnd = 136;
  move->moveToY = (s16)(int)sprites[52]->posY;
  self->detailOffsetX = (float)(move->slideDelta - move->slideStart);
  self->detailScaleEnd = 0.8f;
  self->detailOffsetY = (float)(move->moveToY - move->slideEnd);
  move->t = 0.0f;
  self->detailScale = sprites[cursor + 13]->scaleX;
}
