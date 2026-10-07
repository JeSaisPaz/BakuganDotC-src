// bdc 0x089291b0 UiHologramViewSetField
#include "bdc.h"

/* Slot-5 override (`SetField`) of the hologram view screen (task id 392, vtable `0x08af4a44`):
   indices 0-2 go to `CoreTaskSetField`; index 3 sets the phase (`+0x28`) and resets `phaseStep`
   (`+0x2c`) when the value changes. */

void UiHologramViewSetField(UiHologramView *self, u32 index, u32 value)

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

