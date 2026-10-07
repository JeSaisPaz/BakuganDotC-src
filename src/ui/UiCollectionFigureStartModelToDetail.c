// bdc 0x0899052c UiCollectionFigureStartModelToDetail
#include "bdc.h"

/* Prepares the move of the selected cell model of the figure collection screen (task 314,
   `maybe_UiScreen314Ctor`; 3x2 grid pages of collected metal figures shown as 3D models
   (`"00_P_Dragonoid_N_U_figure.gmo"`…, environment map `"figure_refmap"`), names
   `"cha_spherename_colle_%02d"`, help `"DWCollectionHelp"`; cursor `+0xe78`, page `+0xe79`,
   category `+0xe7d`, entry lists `+0x11c0`) into the detail view (`back` = 0) or back: converts
   the cell anchor and the detail anchor (sprite 55 of `data`, +64 px) to centred coordinates
   (`UiScreenToCentered`) and stores them as from/to in `cellTweens[detailCell]` (from x/y in
   `slideStart`/`slideEnd`, to x/y in `slideDelta`/`moveToY`), the delta in `moveDx/moveDy`,
   the from/to scales (`baseScale` at the cell, `baseScale` * detail scale of the entry in the
   detail view) and resets the progress `t`. */

void UiCollectionFigureStartModelToDetail(UiCollectionFigure *self, bool back)

{
  GfxSprite *anchor;
  UiTween *move;
  float detailScale;
  float cellPos[2];
  float detailPos[2];

  if (back == 0) {
    UiScreenToCentered(self->cellAnchor[self->detailCell][0],
                       self->cellAnchor[self->detailCell][1], cellPos);
    move = &self->cellTweens[self->detailCell];
    move->slideStart = (s16)(int)cellPos[0];
    move->slideEnd = (s16)(int)cellPos[1];
    anchor = ((GfxSprite **)self->base.data)[55];
    UiScreenToCentered(anchor->posX, anchor->posY + 64.0f, detailPos);
    move = &self->cellTweens[self->detailCell];
    move->slideDelta = (s16)(int)detailPos[0];
    move->moveToY = (s16)(int)detailPos[1];
    self->moveDx = (float)(move->slideDelta - move->slideStart);
    self->moveDy = (float)(move->moveToY - move->slideEnd);
    self->moveScaleFrom = self->baseScale;
    detailScale = UiCollectionFigureGetDetailScale(
        self, 0, self->entryIds[self->page * 6 + self->cursor]);
    self->moveScaleTo = self->baseScale * detailScale;
  } else {
    UiScreenToCentered(self->cellAnchor[self->detailCell][0],
                       self->cellAnchor[self->detailCell][1], cellPos);
    move = &self->cellTweens[self->detailCell];
    move->slideDelta = (s16)(int)cellPos[0];
    move->moveToY = (s16)(int)cellPos[1];
    anchor = ((GfxSprite **)self->base.data)[55];
    UiScreenToCentered(anchor->posX, anchor->posY + 64.0f, detailPos);
    move = &self->cellTweens[self->detailCell];
    move->slideStart = (s16)(int)detailPos[0];
    move->slideEnd = (s16)(int)detailPos[1];
    self->moveDx = (float)(move->slideDelta - move->slideStart);
    self->moveDy = (float)(move->moveToY - move->slideEnd);
    self->moveScaleTo = self->baseScale;
    detailScale = UiCollectionFigureGetDetailScale(
        self, 0, self->entryIds[self->page * 6 + self->cursor]);
    self->moveScaleFrom = self->baseScale * detailScale;
  }
  self->cellTweens[self->detailCell].t = 0.0f;
}
