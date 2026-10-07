// bdc 0x0892eafc UiBakuganSelectReleasePedestal
#include "bdc.h"

/* Releases the pedestal model `+0x1cfc` of the Bakugan select screen (`UiBakuganSelectCtor`, task
   371; cursor `+0x74`, current entry `+0x75`, owned list `+0x1ba4` with 0xc-byte entries)
   (`CoreObjectDeferDelete`). */

void UiBakuganSelectReleasePedestal(UiBakuganSelect *self)

{
  if (self->pedestal != (CoreObject *)0x0) {
    CoreObjectDeferDelete(self->pedestal,0);
    self->pedestal = (void *)0x0;
  }
  return;
}

