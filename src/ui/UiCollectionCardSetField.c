// bdc 0x089826dc UiCollectionCardSetField
#include "bdc.h"

/* Slot-5 override (`SetField`) of the `UiCollectionCard` screen, a compiled copy of
   `UiConfirmDialogSetField`: indices 0-2 go to `CoreTaskSetField`; index 3 sets the phase
   (`+0x28`) and resets `phaseStep` (`+0x2c`) when the value changes. */

void UiCollectionCardSetField(UiScreen *screen, u32 index, u32 value)
{
    if (index < 3) {
        CoreTaskSetField(&screen->base, index, value);
        return;
    }
    if ((index == 3) && (screen->phase != value)) {
        screen->phase = value;
        screen->phaseStep = 0;
    }
}
