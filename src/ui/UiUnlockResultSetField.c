// bdc 0x08938094 UiUnlockResultSetField
#include "bdc.h"

/* Slot-5 override (`SetField`) of the UnlockResult screen (task id 375, vtable `0x08af4b24`):
   indices 0-2 go to `CoreTaskSetField`; index 3 sets the phase (`+0x28`) and resets `phaseStep`
   (`+0x2c`) when the value changes. */

void UiUnlockResultSetField(UiUnlockResult *self, u32 index, u32 value)

{
  if (index < 3) {
    CoreTaskSetField((CoreTask *)self,index,value);
    return;
  }
  if ((index == 3) && ((self->base).phase != value)) {
    (self->base).phase = value;
    (self->base).phaseStep = 0;
  }
  return;
}

