// bdc 0x08941850 UiNetLobbyGetField
#include "bdc.h"

/* Slot-6 override (`GetField`) of the ad-hoc lobby screen (task 2000): indices 0-2 come from
   `CoreTaskGetField`; 3 returns the phase, 4 returns `phaseStep`; others 0. */
u32 UiNetLobbyGetField(UiNetLobby *self, u32 index)
{
    if (index < 3) {
        return CoreTaskGetField((CoreTask *)self, index);
    }
    if (index == 3) {
        return self->base.phase;
    }
    if (index == 4) {
        return self->base.phaseStep;
    }
    return 0;
}
