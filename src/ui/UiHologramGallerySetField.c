// bdc 0x0891c2d8 UiHologramGallerySetField
#include "bdc.h"

/* Slot-5 override (`SetField`) of the hologram gallery screen (task id 391, vtable `0x08af4a0c`):
   indices 0-2 go to `CoreTaskSetField`; index 3 sets the phase (`+0x28`) and resets `phaseStep`
   (`+0x2c`) when the value changes. */

void UiHologramGallerySetField(UiHologramGallery *self, u32 index, u32 value)

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

