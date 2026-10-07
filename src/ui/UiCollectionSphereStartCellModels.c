// bdc 0x0897f534 UiCollectionSphereStartCellModels
#include "bdc.h"

/* Starts the zoom of the cell models of the sphere (Bakugan figure) collection screen (task 312,
   `maybe_UiScreen312Ctor`; 3x2 grid pages of collected Bakugan shown as 3D models
   (`"00_P_Dragonoid_N_P.gmo"`…), names `"cha_spherename_colle_%02d"`, pop-out motions
   (`"00_dor_dir_popout"`…); cursor `+0xee0`, page `+0xee1`, category `+0xee5`, entry lists
   `+0x1250`). `out == 0` (zoom in): releases every cell model
   (`UiCollectionSphereReleaseCellModel`), loads (`UiCollectionSphereLoadCellModel`) the models
   of the used cells of the current page (grid pages: `entryIds`; category-2 kind-1/2 pages: the one
   model of slot 6 from `kind1Ids`/`kind2Ids`) and arms each at alpha 0 with its type and start
   scale. `out != 0` (zoom out): arms every loaded model at alpha 1 with the type's start scale. */

void UiCollectionSphereStartCellModels(UiCollectionSphere *self, u8 out)
{
    UiCollectionSphereCell *rec;
    s8 category;
    s8 page;
    u8 kind;
    u8 id;
    int cell;

    kind = UiCollectionSphereGetPageKind(self, (u8)self->page);
    if (out == 0) {
        for (cell = 0; cell < 7; cell++) {
            rec = &self->cells[cell];
            UiCollectionSphereReleaseCellModel(self, (u8)cell);
            if (kind == 0xff || kind == 0) {
                category = self->category;
                page = self->page;
                if (UiCollectionSphereIsCellUsed(self, (u8)category, (u8)cell, (u8)page) != true) {
                    continue;
                }
                if (category == 2) {
                    id = self->entryIds[(page / 3) * 6 + cell];
                } else {
                    id = self->entryIds[page * 6 + cell];
                }
                if (id == 0) {
                    continue;
                }
                UiCollectionSphereLoadCellModel(self, (u8)category, id, (u8)page, (u8)cell);
                self->models[cell]->ambient[3] = 0.0f;
                rec->moveT = 0.0f;
                rec->alphaFrom = 0.0f;
                rec->type = 0;
                rec->scaleFrom = 0.6f;
            } else if (cell == 6) {
                if (kind < 2) {
                    page = self->page;
                    if (self->kind1Ids[page / 3] == 0) {
                        continue;
                    }
                    UiCollectionSphereLoadCellModel(self, (u8)self->category,
                                                    self->kind1Ids[page / 3], (u8)page, (u8)cell);
                    self->models[cell]->ambient[3] = 0.0f;
                    rec->moveT = 0.0f;
                    rec->alphaFrom = 0.0f;
                    rec->type = 0;
                    rec->scaleFrom = 0.6f;
                } else if (kind < 3) {
                    page = self->page;
                    if (self->kind2Ids[page / 3] == 0) {
                        continue;
                    }
                    UiCollectionSphereLoadCellModel(self, (u8)self->category,
                                                    self->kind2Ids[page / 3], (u8)page, (u8)cell);
                    self->models[cell]->ambient[3] = 0.0f;
                    page = self->page;
                    rec->moveT = 0.0f;
                    rec->alphaFrom = 0.0f;
                    if (page / 3 == 0) {
                        rec->type = 1;
                        rec->scaleFrom = 0.27f;
                    } else {
                        rec->type = 2;
                        rec->scaleFrom = 0.3f;
                    }
                }
            }
        }
    } else {
        for (cell = 0; cell < 7; cell++) {
            if (self->models[cell] == NULL) {
                continue;
            }
            rec = &self->cells[cell];
            self->models[cell]->ambient[3] = 1.0f;
            rec->moveT = 0.0f;
            rec->alphaFrom = 1.0f;
            switch (rec->type) {
            case 0:
                rec->scaleFrom = 0.4f;
                break;
            case 1:
                rec->scaleFrom = 0.18f;
                break;
            case 2:
                rec->scaleFrom = 0.2f;
                break;
            }
        }
    }
}
