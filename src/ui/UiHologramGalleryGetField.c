// bdc 0x0891c33c UiHologramGalleryGetField
#include "bdc.h"

/* Slot-6 override (`GetField`) of the hologram gallery screen (task id 391, vtable `0x08af4a0c`):
   indices 0-2 come from `CoreTaskGetField`; index 3 returns the phase (`+0x28`); other indices
   return 0. */

u32 UiHologramGalleryGetField(UiHologramGallery *self, u32 index)

{
  u32 result;

  result = 0;
  if (index < 3) {
    return CoreTaskGetField((CoreTask *)self,index);
  }
  if (index == 3) {
    result = (self->base).phase;
  }
  return result;
}
