// bdc 0x0890aef8 UiLoadingSetField
#include "bdc.h"

/* `SetField` (vtable slot `+0x2c`) of the now-loading screen (task 10100 / 0x2774, 0x240 bytes,
   vtable `0x08af47dc`, `UiLoadingCtor`; shared UI objects `0x08ac0e80`): indices 0-2 go to
   `CoreTaskSetField`; index 3 sets the state `+0x10` and resets the step `+0x14` when it changes.
    */

void UiLoadingSetField(UiLoading *self, u32 index, u32 value)

{
  if (index < 3) {
    CoreTaskSetField(&self->base,index,value);
    return;
  }
  if ((index == 3) && (self->state != value)) {
    self->state = value;
    self->step = 0;
  }
  return;
}

