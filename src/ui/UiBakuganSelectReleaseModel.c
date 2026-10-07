// bdc 0x0892d0e4 UiBakuganSelectReleaseModel
#include "bdc.h"

/* Releases the Bakugan model `+0x1cf8` of the Bakugan select screen (`UiBakuganSelectCtor`, task
   371; cursor `+0x74`, current entry `+0x75`, owned list `+0x1ba4` with 0xc-byte entries)
   (`CoreObjectDeferDelete`). */

void UiBakuganSelectReleaseModel(UiBakuganSelect *self)

{
  if (self->model != (CoreObject *)0x0) {
    CoreObjectDeferDelete(self->model,0);
    self->model = (void *)0x0;
  }
  return;
}

