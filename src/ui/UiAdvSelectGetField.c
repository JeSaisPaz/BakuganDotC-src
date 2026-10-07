// bdc 0x08917acc UiAdvSelectGetField
#include "bdc.h"

/* Slot-6 override (`GetField`) of the AdvSelect screen (task id 376, vtable `0x08af49d4`): indices
   0-2 come from `CoreTaskGetField`; index 3 returns the phase (`+0x28`); other indices return 0.
    */

u32 UiAdvSelectGetField(UiAdvSelect *self, u32 index)

{
  u32 result;
  
  result = 0;
  if (index < 3) {
    result = CoreTaskGetField((CoreTask *)self,index);
    return result;
  }
  if (index == 3) {
    result = (self->base).phase;
  }
  return result;
}

