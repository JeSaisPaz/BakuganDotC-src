// bdc 0x0892b524 UiBakuganSelectFreeModels
#include "bdc.h"

/* Deletes the Bakugan model `+0x1cf8` and the pedestal `+0x1cfc` of the Bakugan select screen
   (`UiBakuganSelectCtor`, task 371; cursor `+0x74`, current entry `+0x75`, owned list `+0x1ba4`
   with 0xc-byte entries) through their virtual destructors. */

static void DestroyVirtual(void *obj)
{
  const VtblEntry *entry = (const VtblEntry *)((CoreObject *)obj)->vtable + 1;

  ((void (*)(void *, int))entry->fn)((char *)obj + entry->delta, 3);
}

void UiBakuganSelectFreeModels(UiBakuganSelect *self)

{
  if (self->model != (void *)0x0) {
    DestroyVirtual(self->model);
    self->model = (void *)0x0;
  }
  if (self->pedestal != (void *)0x0) {
    DestroyVirtual(self->pedestal);
    self->pedestal = (void *)0x0;
  }
}
