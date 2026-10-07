// bdc 0x08973964 UiCollectionMenuUpdateItemBox
#include "bdc.h"

/* Per-frame update of the item-box model of `UiCollectionMenu` while it is
   active (`+0x520`): advances its motion by 0.5 frame and updates it (model vtable slots at
   +0x34/+0x3c). */

void UiCollectionMenuUpdateItemBox(UiCollectionMenu *self)

{
  if ((self->itemBoxClosing != 0) && (self->itemBox != (GfxModel *)0x0)) {
    const VtblEntry *a = &((const VtblEntry *)self->itemBox->base.vtable)[6];
    ((void (*)(void *, float))a->fn)((u8 *)self->itemBox + a->delta, 0.5f);
    const VtblEntry *u = &((const VtblEntry *)self->itemBox->base.vtable)[7];
    ((void (*)(void *))u->fn)((u8 *)self->itemBox + u->delta);
  }
}
