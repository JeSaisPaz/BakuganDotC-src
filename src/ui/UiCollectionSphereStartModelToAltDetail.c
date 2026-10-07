// bdc 0x08980e7c UiCollectionSphereStartModelToAltDetail
#include "bdc.h"

/* Starts the move of the detailCell model of `UiCollectionSphere`
   between its cell (sprite 57 position + entry-type y offset, centred) and the motion-view spot
   (240, 136 + entry-type offset, centred, with y shifted by the detail offset x of
   `UiCollectionSphereGetDetailOffset`). `back == false` moves cell -> spot, `back == true`
   spot -> cell. Sets the record's from/to, `detailMoveDelta = to - from`, the scales (detail scale
   * 1.4 at the cell end, detail scale * detail offset y at the spot end) and `moveT = 0`. */

void UiCollectionSphereStartModelToAltDetail(UiCollectionSphere *self, bool back)
{
    float cellPos[2];
    float spotPos[2];
    float offset[2];
    float x;
    float typeOffset;
    UiCollectionSphereCell *cell;

    if (back) {
        x = ((GfxSprite **)self->base.data)[57]->posX;
        typeOffset = UiCollectionSphereGetEntryTypeOffset(self);
        UiScreenToCentered(x, ((GfxSprite **)self->base.data)[57]->posY + typeOffset, cellPos);
        cell = &self->cells[self->detailCell];
        cell->toX = (s16)(int)cellPos[0];
        cell->toY = (s16)(int)cellPos[1];
        typeOffset = UiCollectionSphereGetEntryTypeOffset(self);
        UiScreenToCentered(240.0f, typeOffset + 136.0f, spotPos);
        UiCollectionSphereGetDetailOffset(offset, &self->base);
        cell = &self->cells[self->detailCell];
        cell->fromX = (s16)(int)spotPos[0];
        cell->fromY = (s16)(int)(offset[0] + spotPos[1]);
        self->detailMoveDelta[0] = (float)(cell->toX - cell->fromX);
        self->detailMoveDelta[1] = (float)(cell->toY - cell->fromY);
        self->detailScaleTo = UiCollectionSphereGetDetailScale(self, (u8)self->detailCell) * 1.4f;
        self->detailScaleFrom = UiCollectionSphereGetDetailScale(self, (u8)self->detailCell) * offset[1];
        self->cells[self->detailCell].moveT = 0.0f;
    } else {
        x = ((GfxSprite **)self->base.data)[57]->posX;
        typeOffset = UiCollectionSphereGetEntryTypeOffset(self);
        UiScreenToCentered(x, ((GfxSprite **)self->base.data)[57]->posY + typeOffset, cellPos);
        cell = &self->cells[self->detailCell];
        cell->fromX = (s16)(int)cellPos[0];
        cell->fromY = (s16)(int)cellPos[1];
        typeOffset = UiCollectionSphereGetEntryTypeOffset(self);
        UiScreenToCentered(240.0f, typeOffset + 136.0f, spotPos);
        UiCollectionSphereGetDetailOffset(offset, &self->base);
        cell = &self->cells[self->detailCell];
        cell->toX = (s16)(int)spotPos[0];
        cell->toY = (s16)(int)(offset[0] + spotPos[1]);
        self->detailMoveDelta[0] = (float)(cell->toX - cell->fromX);
        self->detailMoveDelta[1] = (float)(cell->toY - cell->fromY);
        self->detailScaleFrom = UiCollectionSphereGetDetailScale(self, (u8)self->detailCell) * 1.4f;
        self->detailScaleTo = UiCollectionSphereGetDetailScale(self, (u8)self->detailCell) * offset[1];
        self->cells[self->detailCell].moveT = 0.0f;
    }
}
