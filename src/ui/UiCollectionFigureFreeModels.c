// bdc 0x0898b384 UiCollectionFigureFreeModels
#include "bdc.h"

/* Deletes the six cell models of `UiCollectionFigure` (`models[6]`,
   virtual destructor flag 3); called by `UiCollectionFigureDtor`. */

void UiCollectionFigureFreeModels(UiCollectionFigure *self)
{
    int i;

    for (i = 0; i < 6; i++) {
        GfxModel *model = self->models[i];

        if (model != NULL) {
            const VtblEntry *dtor = &((const VtblEntry *)model->base.vtable)[1];

            ((void (*)(void *, int))dtor->fn)((u8 *)model + dtor->delta, 3);
            self->models[i] = NULL;
        }
    }
}
