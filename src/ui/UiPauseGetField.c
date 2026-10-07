// bdc 0x08910434 UiPauseGetField
#include "bdc.h"

/* Slot-6 override (`GetField`) of the Pause screen (task id 410, vtable `0x08af4964`): indices 0-2
   come from `CoreTaskGetField`; index 3 returns the phase (`+0x28`); other indices return 0. */

u32 UiPauseGetField(UiPause *self, u32 index)
{
    if (index < 3) {
        return CoreTaskGetField((CoreTask *)self, index);
    }
    if (index == 3) {
        return self->base.phase;
    }
    return 0;
}
