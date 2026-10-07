// bdc 0x0892b74c UiBakuganSelectUpdateModel
#include "bdc.h"

/* Calls the update slot `+0x3c` of the Bakugan model `+0x1cf8` of the Bakugan select screen
   (`UiBakuganSelectCtor`, task 371; cursor `+0x74`, current entry `+0x75`, owned list `+0x1ba4`
   with 0xc-byte entries) when it exists. */

void UiBakuganSelectUpdateModel(UiBakuganSelect *self)

{
  void *model = self->model;

  if (model != NULL) {
    const VtblEntry *update = &((const VtblEntry *const *)model)[5][7];

    ((void (*)(void *))update->fn)((u8 *)model + update->delta);
  }
  return;
}
