// bdc 0x0897376c UiCollectionMenuFreeItemBox
#include "bdc.h"

/* Deletes the rotating item-box model `itemBox` of `UiCollectionMenu`
   through its deleting destructor (vtable entry 1, flag 3) and clears the pointer (the binary
   stores NULL twice); does nothing when there is no model. Called by `UiCollectionMenuDtor`. */
void UiCollectionMenuFreeItemBox(UiCollectionMenu *self)
{
    GfxModel *itemBox = self->itemBox;

    if (itemBox != NULL) {
        const VtblEntry *dtor = &((const VtblEntry *)itemBox->base.vtable)[1];

        ((void (*)(void *, s32))dtor->fn)((u8 *)itemBox + dtor->delta, 3);
        self->itemBox = NULL;
        self->itemBox = NULL;
    }
}
