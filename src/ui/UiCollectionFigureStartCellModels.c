// bdc 0x0898d2ac UiCollectionFigureStartCellModels
#include "bdc.h"

/* Starts the zoom of the six cell models of `UiCollectionFigure`. On the
   way in (`out` = 0) clears each cell (`UiCollectionFigureReleaseCellModel`) and, for every used cell
   (`UiCollectionFigureIsCellUsed`) with a non-zero entry id in the current page's list, loads its
   model (`UiCollectionFigureLoadCellModel`) at scale 0 and resets its zoom record (target 0.12);
   on the way out sets every loaded model's scale to 1 and the record to shrink back. */

void UiCollectionFigureStartCellModels(UiCollectionFigure *self, u8 out)
{
    int i;

    if (out == 0) {
        for (i = 0; i < 6; i++) {
            u8 cell = (u8)i;
            u8 category;
            s8 page;
            u8 id;

            UiCollectionFigureReleaseCellModel(self, cell);
            category = (u8)self->category;
            page = self->page;
            if (UiCollectionFigureIsCellUsed(self, category, cell, (u8)page) == 1) {
                id = self->entryIds[page * 6 + i];
                if (id != 0) {
                    UiCollectionFigureLoadCellModel(self, category, id, (u8)page, cell);
                    self->models[i]->ambient[3] = 0.0f;
                    self->cellTweens[i].t = 0.0f;
                    self->cellTweens[i].startAlpha = 0.0f;
                    self->cellTweens[i].type08 = 0;
                    self->cellTweens[i].startScale = 0.12f;
                }
            }
        }
    } else {
        for (i = 0; i < 6; i++) {
            GfxModel *model = self->models[i];

            if (model != NULL) {
                model->ambient[3] = 1.0f;
                self->cellTweens[i].t = 0.0f;
                self->cellTweens[i].startAlpha = 1.0f;
                self->cellTweens[i].startScale = self->baseScale;
            }
        }
    }
}
