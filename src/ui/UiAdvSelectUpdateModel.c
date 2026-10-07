// bdc 0x08917b20 UiAdvSelectUpdateModel
#include "bdc.h"

/* Calls the update slot `+0x3c` of the Bakugan model `+0x914` of the adventure partner-select
   screen (`UiAdvSelectCtor`, task 376; cursor `+0x74`, candidates `+0x8a0` as 4-byte `{id,
   partner, locked, ?}` slots) when it exists. */

void UiAdvSelectUpdateModel(UiAdvSelect *self)

{
  GfxModel *model = (GfxModel *)self->model;

  if (model != NULL) {
    const VtblEntry *update = &((const VtblEntry *)model->base.vtable)[7];

    ((void (*)(void *))update->fn)((u8 *)model + update->delta);
  }
}
