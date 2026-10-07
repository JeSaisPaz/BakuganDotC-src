// bdc 0x08909db4 UiScreenSetFrameMode
#include "bdc.h"

/* Configures frame pacing and pad repeat for a screen: sets `g_gfxDisplay->frameSkip = mode` and
   the pad repeat delay/interval from the table delay `{0x20,0x10,10,8,4,2,1,1}[mode]`, interval
   `{...}[mode+4]` (so mode 0 gives delay 0x20 / interval 4, mode 1 gives 0x10 / 2). The first
   argument is unused. */

void UiScreenSetFrameMode(CoreTask *screen, s32 mode)
{
    s32 table[8];

    table[0] = 0x20;
    table[1] = 0x10;
    table[2] = 10;
    table[3] = 8;
    table[4] = 4;
    table[5] = 2;
    g_gfxDisplay->frameSkip = mode;
    table[6] = 1;
    table[7] = 1;
    PadSetRepeatDelay(g_padState, (u16)table[mode]);
    PadSetRepeatInterval(g_padState, (u16)table[mode + 4]);
}
