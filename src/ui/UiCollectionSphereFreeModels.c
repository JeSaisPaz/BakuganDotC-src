// bdc 0x08979300 UiCollectionSphereFreeModels
#include "bdc.h"

/* Deletes the seven cell/detail models of `UiCollectionSphere`
   (`models[7]`, virtual destructor flag 3) and frees all loaded motions (`GmoMotionFreeAll`);
   called by `UiCollectionSphereDtor`. */

void UiCollectionSphereFreeModels(UiCollectionSphere *self)
{
    int i;

    for (i = 0; i < 7; i++) {
        GfxModel *model = self->models[i];

        if (model != NULL) {
            const VtblEntry *dtor = &((const VtblEntry *)model->base.vtable)[1];

            ((void (*)(void *, int))dtor->fn)((u8 *)model + dtor->delta, 3);
            self->models[i] = NULL;
        }
    }
    GmoMotionFreeAll(GmoMotionMgrGet(), 0);
}
