// bdc 0x0898b510 UiCollectionFigureSetField
#include "bdc.h"

/* Slot-5 override (`SetField`) of the `UiCollectionFigure` screen, a compiled copy of
   `UiConfirmDialogSetField`: indices 0-2 go to `CoreTaskSetField`; index 3 sets the phase
   (`+0x28`) and resets `phaseStep` (`+0x2c`) when the value changes. */

void UiCollectionFigureSetField(UiScreen *screen, u32 index, u32 value)
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
