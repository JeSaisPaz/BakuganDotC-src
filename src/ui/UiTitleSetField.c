// bdc 0x08950bf0 UiTitleSetField
#include "bdc.h"

/* Byte-identical compiled copy of `UiConfirmDialogSetField` for `UiTitle` (same instructions up
   to relocated addresses): indices 0-2 go to `CoreTaskSetField`; index 3 sets the phase (`+0x28`)
   and resets `phaseStep` (`+0x2c`) when the value changes. */

void UiTitleSetField(UiScreen *screen, u32 index, u32 value)

{
  if (index < 3) {
    CoreTaskSetField(&screen->base,index,value);
    return;
  }
  if ((index == 3) && (screen->phase != value)) {
    screen->phase = value;
    screen->phaseStep = 0;
  }
  return;
}
