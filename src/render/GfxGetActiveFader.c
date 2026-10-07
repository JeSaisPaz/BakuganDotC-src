// bdc 0x089edb50 GfxGetActiveFader
#include "bdc.h"

/* Returns the currently active full-screen fade overlay (the second word of `g_faderSlots`).
   Callers poke its colours directly (`+0x30` start RGBA, `+0x40` end RGBA, as float vec4) and then
   start it with `GfxFaderStart` / poll `GfxFaderIsFinished` (finished flag). */

GfxFader * GfxGetActiveFader(void)

{
  return g_faderSlots[1];
}

