// bdc 0x08980648 UiCollectionSphereStartModelToDetail
#include "bdc.h"

/* Starts the move of the detailCell model of `UiCollectionSphere`
   between its grid cell (cellPos + cameraY after UiCollectionSphereSetCellYOffset, centred) and
   the detail anchor (sprite 57 position + entry-type y offset, centred). `back == false` moves
   cell -> anchor, `back == true` anchor -> cell. Sets the record's from/to,
   `detailMoveDelta = to - from`, the scales (detail scale at the cell end, detail scale * 1.4 at
   the anchor end) and `moveT = 0`. */

void UiCollectionSphereStartModelToDetail(UiCollectionSphere *self, bool back)
{
    float cellPos[2];
    float anchorPos[2];
    float x;
    float typeOffset;
    float scale;
    UiCollectionSphereCell *cell;

    if (!back) {
        UiCollectionSphereSetCellYOffset(self, (u8)self->page, (u8)self->detailCell);
        UiScreenToCentered(self->cellPos[self->detailCell][0],
                           self->cellPos[self->detailCell][1] + self->cameraY, cellPos);
        cell = &self->cells[self->detailCell];
        cell->fromX = (s16)(int)cellPos[0];
        cell->fromY = (s16)(int)cellPos[1];
        x = ((GfxSprite **)self->base.data)[57]->posX;
        typeOffset = UiCollectionSphereGetEntryTypeOffset(self);
        UiScreenToCentered(x, ((GfxSprite **)self->base.data)[57]->posY + typeOffset, anchorPos);
        cell = &self->cells[self->detailCell];
        cell->toX = (s16)(int)anchorPos[0];
        cell->toY = (s16)(int)anchorPos[1];
        self->detailMoveDelta[0] = (float)(cell->toX - cell->fromX);
        self->detailMoveDelta[1] = (float)(cell->toY - cell->fromY);
        scale = UiCollectionSphereGetDetailScale(self, (u8)self->detailCell);
        self->detailScaleFrom = scale;
        self->detailScaleTo = scale * 1.4f;
        self->cells[self->detailCell].moveT = 0.0f;
    } else {
        UiCollectionSphereSetCellYOffset(self, (u8)self->page, (u8)self->detailCell);
        UiScreenToCentered(self->cellPos[self->detailCell][0],
                           self->cellPos[self->detailCell][1] + self->cameraY, cellPos);
        cell = &self->cells[self->detailCell];
        cell->toX = (s16)(int)cellPos[0];
        cell->toY = (s16)(int)cellPos[1];
        x = ((GfxSprite **)self->base.data)[57]->posX;
        typeOffset = UiCollectionSphereGetEntryTypeOffset(self);
        UiScreenToCentered(x, ((GfxSprite **)self->base.data)[57]->posY + typeOffset, anchorPos);
        cell = &self->cells[self->detailCell];
        cell->fromX = (s16)(int)anchorPos[0];
        cell->fromY = (s16)(int)anchorPos[1];
        self->detailMoveDelta[0] = (float)(cell->toX - cell->fromX);
        self->detailMoveDelta[1] = (float)(cell->toY - cell->fromY);
        scale = UiCollectionSphereGetDetailScale(self, (u8)self->detailCell);
        self->detailScaleTo = scale;
        self->detailScaleFrom = scale * 1.4f;
        self->cells[self->detailCell].moveT = 0.0f;
    }
}
