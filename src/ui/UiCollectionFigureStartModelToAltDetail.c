// bdc 0x08990d48 UiCollectionFigureStartModelToAltDetail
#include "bdc.h"

/* Prepares the move of the detail model of the figure collection screen (task 314,
   `maybe_UiScreen314Ctor`) between the detail view and the second detail view: like
   `UiCollectionFigureStartModelToDetail`, but the move runs from the detail anchor (sprite 55
   of `data`, +64 px) to the second detail anchor (240, 200) when `back` is 0, and the other way
   round otherwise (both converted with `UiScreenToCentered` and stored as from/to in
   `cellTweens[detailCell]`, delta in `moveDx/moveDy`). The scales go from `baseScale` * detail
   scale (view 0) to `baseScale` * second detail scale (view 1) * `zoom`, or back; the progress
   `t` is reset. */

void UiCollectionFigureStartModelToAltDetail(UiCollectionFigure *self, bool back)
{
  GfxSprite *anchor;
  UiTween *move;
  float scale;
  float scale2;
  float fromPos[2];
  float toPos[2];
  float backFrom[2];
  float backTo[2];

  anchor = ((GfxSprite **)self->base.data)[55];
  if (back == 0) {
    UiScreenToCentered(anchor->posX, anchor->posY + 64.0f, fromPos);
    move = &self->cellTweens[self->detailCell];
    move->slideStart = (s16)(int)fromPos[0];
    move->slideEnd = (s16)(int)fromPos[1];
    UiScreenToCentered(240.0f, 200.0f, toPos);
    move = &self->cellTweens[self->detailCell];
    move->slideDelta = (s16)(int)toPos[0];
    move->moveToY = (s16)(int)toPos[1];
    self->moveDx = (float)(move->slideDelta - move->slideStart);
    self->moveDy = (float)(move->moveToY - move->slideEnd);
    scale = UiCollectionFigureGetDetailScale(
        self, 0, self->entryIds[self->page * 6 + self->cursor]);
    self->moveScaleFrom = self->baseScale * scale;
    scale2 = UiCollectionFigureGetDetailScale(
        self, 1, self->entryIds[self->page * 6 + self->cursor]);
    scale2 = self->baseScale * scale2 * self->zoom;
    self->cellTweens[self->detailCell].t = 0.0f;
    self->moveScaleTo = scale2;
  } else {
    UiScreenToCentered(anchor->posX, anchor->posY + 64.0f, backTo);
    move = &self->cellTweens[self->detailCell];
    move->slideDelta = (s16)(int)backTo[0];
    move->moveToY = (s16)(int)backTo[1];
    UiScreenToCentered(240.0f, 200.0f, backFrom);
    move = &self->cellTweens[self->detailCell];
    move->slideStart = (s16)(int)backFrom[0];
    move->slideEnd = (s16)(int)backFrom[1];
    self->moveDx = (float)(move->slideDelta - move->slideStart);
    self->moveDy = (float)(move->moveToY - move->slideEnd);
    scale = UiCollectionFigureGetDetailScale(
        self, 0, self->entryIds[self->page * 6 + self->cursor]);
    self->moveScaleTo = self->baseScale * scale;
    scale2 = UiCollectionFigureGetDetailScale(
        self, 1, self->entryIds[self->page * 6 + self->cursor]);
    scale2 = self->baseScale * scale2 * self->zoom;
    self->cellTweens[self->detailCell].t = 0.0f;
    self->moveScaleFrom = scale2;
  }
}
