// bdc 0x089103d0 UiPauseSetField
#include "bdc.h"

/* Slot-5 override (`SetField`) of the Pause screen (task id 410, vtable `0x08af4964`): indices 0-2
   go to `CoreTaskSetField`; index 3 sets the phase (`+0x28`) and resets `phaseStep` (`+0x2c`)
   when the value changes. */

void UiPauseSetField(UiPause *self, u32 index, u32 value)

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

