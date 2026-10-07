// bdc 0x08940468 UiScreen390GetField
#include "bdc.h"

/* Slot-6 override (`GetField`) of `UiScreen390` (terminal-use cutscene, task id
   390, vtable `0x08af4b94`): indices 0-2 come from `CoreTaskGetField`; index 3 returns the phase
   (`+0x28`); other indices return 0. */

u32 UiScreen390GetField(UiScreen *screen, u32 index)
{
    if (index < 3) {
        return CoreTaskGetField(&screen->base, index);
    }
    if (index == 3) {
        return screen->phase;
    }
    return 0;
}
